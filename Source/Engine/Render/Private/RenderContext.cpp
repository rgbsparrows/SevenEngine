#include "RDI/RDIModule.h"
#include "Render/RenderContext.h"
#include "RDI/RDIFunctionHelper.h"
#include "RDI/Interface/RDIDevice.h"
#include "RDI/Interface/RDIFactory.h"
#include "RDI/Interface/RDICommandList.h"
#include "RDI/Interface/RDICommandQueue.h"
#include "RDI/Interface/RDICommandAllocator.h"

void SRenderContext::Init(size_t _subRenderThreadCount) noexcept
{
	mFactory = GetRDIModule()->GetRDIFactory();
	mDevice = mFactory->GetDevice();
	mCommandQueue = mDevice->GetCommandQueue();

	mRenderTaskGraph.Construct(_subRenderThreadCount);

	for (uint32_t i = 0; i != GRenderInfoCount; ++i)
		for (size_t j = 0; j < _subRenderThreadCount; ++j)
			mCommandAllocatorList[i].push_back(mDevice->CreateCommandAllocator(ERDICommandListType::Direct));
}

void SRenderContext::Clear() noexcept
{
	SyncToGpuFrameEnd(true);

	for (uint32_t i = 0; i != GRenderInfoCount; ++i)
		for (size_t j = 0; j != mCommandAllocatorList[i].size(); ++j)
			mCommandAllocatorList[i][j]->Release();
}

void SRenderContext::BeginFrame() noexcept
{
	mCommandQueue->YieldUntilCompletion(GetGpuFence());
	mIsSyncToGpuFrameEnd = false;

	for (uint32_t i = 0; i != mCommandAllocatorList[0].size(); ++i)
		GetCommandAllocator(i)->Reset();
}

void SRenderContext::EndFrame() noexcept
{
	SetGpuFence(mCommandQueue->Signal());
}

SRenderTaskIdentify SRenderContext::AddRenderTask(const std::string& taskName, std::function<void(IRDICommandList*)>&& _renderTask) noexcept
{
	return AddRenderTask(taskName, std::move(_renderTask), GetAllRenderTask());
}

SRenderTaskIdentify SRenderContext::AddRenderTask(const std::string& taskName, std::function<void(IRDICommandList*)>&& _renderTask, const std::vector<SRenderTaskIdentify>& depdency) noexcept
{
	SRenderTaskIdentify taskIdentify(mRenderTaskList.size());

	SRenderTask renderTask;
	renderTask.mTaskName = taskName;
	renderTask.mRenderTask = std::move(_renderTask);
	renderTask.mDependencyTaskList = depdency;

	mRenderTaskList.push_back(std::move(renderTask));

	return taskIdentify;
}

void SRenderContext::ExecuteRenderGraph() noexcept
{
	size_t renderTaskCount = mRenderTaskList.size();

	std::vector<STaskIdentify> taskIdentifyList(renderTaskCount, STaskIdentify());
	std::vector<IRDICommandList*> commandListList(renderTaskCount, nullptr);
	for (size_t i = 0; i != renderTaskCount; ++i)
		commandListList[i] = AllocateCommandList();

	for (size_t i = 0; i != renderTaskCount; ++i)
	{
		std::vector<STaskIdentify> depdencyTaskList;
		for (SRenderTaskIdentify depdencyTaskIdentify : mRenderTaskList[i].mDependencyTaskList)
			depdencyTaskList.push_back(taskIdentifyList[static_cast<uint64_t>(depdencyTaskIdentify)]);

		SRenderTask& renderTask = mRenderTaskList[i];
		taskIdentifyList[i] = mRenderTaskGraph.AddTask(renderTask.mTaskName, [=, &commandListList, &renderTask](size_t _threadIndex)
			{
				IRDICommandList* commandList = commandListList[i];
				commandList->Reset(GetCommandAllocator(static_cast<uint32_t>(_threadIndex)));
				commandList->BeginEvent(renderTask.mTaskName);
				renderTask.mRenderTask(commandList);
				commandList->EndEvent();
				commandList->Close();
			}, depdencyTaskList
		);
	}

	mRenderTaskGraph.WaitTask(taskIdentifyList);

	std::vector<bool> isCommandListExecuted(renderTaskCount, false);
	while (Contains(isCommandListExecuted, false))
	{
		std::vector<IRDICommandList*> couldExecuteCommandListList;
		for (size_t i = 0; i != renderTaskCount; ++i)
		{
			if (isCommandListExecuted[i])
				continue;

			bool isAllDependencyTaskExecuted = true;
			for (SRenderTaskIdentify depdencyTaskIdentify : mRenderTaskList[i].mDependencyTaskList)
			{
				if (!isCommandListExecuted[static_cast<uint64_t>(depdencyTaskIdentify)])
				{
					isAllDependencyTaskExecuted = false;
					break;
				}
			}

			if (isAllDependencyTaskExecuted)
			{
				couldExecuteCommandListList.push_back(commandListList[i]);
				isCommandListExecuted[i] = true;
			}
		}

		CHECK(couldExecuteCommandListList.empty() == false);
		ExecuteCommandLists(static_cast<uint32_t>(couldExecuteCommandListList.size()), couldExecuteCommandListList.data());
	};

	for (size_t i = 0; i != renderTaskCount; ++i)
		ReleaseCommandList(commandListList[i]);

	mRenderTaskList.clear();
}

void SRenderContext::SyncToGpuFrameEnd(bool _force) noexcept
{
	if (mIsSyncToGpuFrameEnd == false || _force == true)
	{
		uint64_t fence = mCommandQueue->Signal();
		mCommandQueue->YieldUntilCompletion(fence);
		mIsSyncToGpuFrameEnd = true;
	}
}

std::vector<SRenderTaskIdentify> SRenderContext::GetAllRenderTask() noexcept
{
	std::vector<SRenderTaskIdentify> taskIdentifyList(mRenderTaskList.size());

	for (size_t i = 0; i != mRenderTaskList.size(); ++i)
		taskIdentifyList.push_back(SRenderTaskIdentify(i));

	return taskIdentifyList;
}

IRDICommandList* SRenderContext::AllocateCommandList() noexcept
{
	if (mCommandListPool.empty())
		return mDevice->CreateCommandList(ERDICommandListType::Direct, GetCommandAllocator(0));

	IRDICommandList* commandList = mCommandListPool.back();
	mCommandListPool.pop_back();

	return commandList;
}

void SRenderContext::ReleaseCommandList(IRDICommandList* _commandList) noexcept
{
	mCommandListPool.push_back(_commandList);
}

IRDICommandAllocator* SRenderContext::GetCommandAllocator(uint32_t _threadIndex) noexcept
{
	return mCommandAllocatorList[GetRenderModule()->GetFrameInfoIndex_RenderThread()][_threadIndex];
}

void SRenderContext::ExecuteCommandLists(uint32_t _commandListCount, IRDICommandList* const* _commandLists) noexcept
{
	mCommandQueue->ExecuteCommandLists(_commandListCount, _commandLists);
}

uint64_t SRenderContext::GetGpuFence() noexcept
{
	return mGpuFence[GetRenderModule()->GetFrameInfoIndex_RenderThread()];
}

void SRenderContext::SetGpuFence(uint64_t _fence) noexcept
{
	mGpuFence[GetRenderModule()->GetFrameInfoIndex_RenderThread()] = _fence;
}

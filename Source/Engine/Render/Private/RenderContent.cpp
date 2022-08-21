#include "Render/RenderContent.h"
#include "RDI/RDIFunctionHelper.h"
#include "RDI/Interface/RDIDevice.h"
#include "RDI/Interface/RDICommandQueue.h"
#include "RDI/Interface/RDICommandAllocator.h"
#include "RDI/Interface/RDICommandList.h"

void SRenderContent::Init(IRDIDevice* _device, IRDICommandQueue* _commandQueue) noexcept
{
	mRDIDevice = _device;
	mRDICommandQueue = _commandQueue;

	for (uint32_t i = 0; i != GRenderInfoCount; ++i)
		mCommandAllocatorList[i].push_back(mRDIDevice->CreateCommandAllocator(ERDICommandListType::Direct));
}

void SRenderContent::Clear() noexcept
{
	SyncToGpuFrameEnd(true);

	for (uint32_t i = 0; i != GRenderInfoCount; ++i)
		for (size_t j = 0; i < mCommandAllocatorList[i].size(); ++j)
			mCommandAllocatorList[i][j]->Release();
}

void SRenderContent::BeginFrame() noexcept
{
	mIsSyncToGpuFrameEnd = false;

	for (uint32_t i = 0; i != mCommandAllocatorList[0].size(); ++i)
		GetCommandAllocator(i)->Reset();
}

void SRenderContent::EndFrame() noexcept
{
}

void SRenderContent::EnsureThreadCount(uint32_t _threadCount) noexcept
{
	for (uint32_t i = 0; i != GRenderInfoCount; ++i)
		for (size_t j = mCommandAllocatorList[0].size(); j < _threadCount; ++j)
			mCommandAllocatorList[i].push_back(mRDIDevice->CreateCommandAllocator(ERDICommandListType::Direct));
}

IRDIDevice* SRenderContent::GetDevice() noexcept
{
	return mRDIDevice;
}

IRDICommandList* SRenderContent::AllocateCommandList(uint32_t _threadIndex) noexcept
{
	if (mCommandListPool.empty())
		return mRDIDevice->CreateCommandList(ERDICommandListType::Direct, GetCommandAllocator(_threadIndex));

	IRDICommandList* commandList = mCommandListPool.back();
	commandList->Reset(GetCommandAllocator(_threadIndex));
	mCommandListPool.pop_back();

	return commandList;
}

void SRenderContent::ReleaseCommandList(IRDICommandList* _commandList) noexcept
{
	mCommandListPool.push_back(_commandList);
}

IRDICommandAllocator* SRenderContent::GetCommandAllocator(uint32_t _threadIndex) noexcept
{
	return mCommandAllocatorList[GetRenderModule()->GetFrameInfoIndex_RenderThread()][_threadIndex];
}

void SRenderContent::ExecuteCommandLists(uint32_t _commandListCount, IRDICommandList* const* _commandLists) noexcept
{
	mRDICommandQueue->ExecuteCommandLists(_commandListCount, _commandLists);
}

void SRenderContent::ExecuteCommandList(IRDICommandList* _commandList) noexcept
{
	mRDICommandQueue->ExecuteCommandLists(1, &_commandList);
}

void SRenderContent::SyncToGpuFrameEnd(bool _force) noexcept
{
	if (mIsSyncToGpuFrameEnd == false || _force == true)
	{
		uint64_t fence = mRDICommandQueue->Signal();
		mRDICommandQueue->YieldUntilCompletion(fence);
		mIsSyncToGpuFrameEnd = true;
	}
}

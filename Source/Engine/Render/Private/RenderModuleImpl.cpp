#include "RDI/RDIModule.h"
#include "DeferFrameTask.h"
#include "RenderModuleImpl.h"
#include "Core/Misc/Thread.h"
#include "Core/Modules/ModuleManager.h"
#include "Render/RenderProxy/Resource/RenderResource.h"

#include <functional>

SRenderModuleImpl* GRenderModuleImpl = nullptr;

IRenderModule* GetRenderModule() noexcept
{
	return GRenderModuleImpl;
}

SRenderModuleImpl* GetRenderModuleImpl() noexcept
{
	return GRenderModuleImpl;
}

IRenderCommandList* GetRenderCommandList() noexcept
{
	return GetRenderModule()->GetRenderCommandList();
}

REGIST_MODULE("RenderModule", SRenderModuleImpl)

bool SRenderModuleImpl::Init() noexcept
{
	GRenderModuleImpl = this;
	SModuleManager::Get().LoadModule("RDIModule");
	mRenderThread = std::thread(std::mem_fn(&SRenderModuleImpl::RenderThreadMain), this);

	return true;
}

void SRenderModuleImpl::Clear() noexcept
{
	{
		//两次循环清空渲染线程全部任务，保证GPU不再使用任何资源
		BeginFrame_GameThread();
		EndFrame_GameThread();

		BeginFrame_GameThread();
		EndFrame_GameThread();

		//推出渲染线程
		BeginFrame_GameThread();
		mRequireExit[GetFrameInfoIndex_GameThread()] = true;
		EndFrame_GameThread();

		mRenderThread.join();
	}

	{
		SModuleManager::Get().UnloadModule("RDIModule");
		GRenderModuleImpl = nullptr;
	}
}

void SRenderModuleImpl::BeginFrame_GameThread() noexcept
{
	Thread::YieldUntilValue(mGameThreadFrameResourceReadyFlag[GetFrameInfoIndex_GameThread()], true);
	mGameThreadFrameResourceReadyFlag[GetFrameInfoIndex_GameThread()] = false;
}

void SRenderModuleImpl::EndFrame_GameThread() noexcept
{
	std::atomic_bool& renderThreadFrameResourceReadyFlag = mRenderThreadFrameResourceReadyFlag[GetFrameInfoIndex_GameThread()];

	++mFrameCount_GameThread;
	mFrameInfoIndex_GameThread = mFrameCount_GameThread % GRenderInfoCount;

	renderThreadFrameResourceReadyFlag = true;
}

void SRenderModuleImpl::Init_RenderThread() noexcept
{
	mRenderContext.Init(Thread::GetSubRenderThreadCount());
	mImguiRenderGraph.Init(mRenderContext);
}

void SRenderModuleImpl::Clear_RenderThread() noexcept
{
	mImguiRenderGraph.Clear(mRenderContext);
	mRenderContext.Clear();
}

void SRenderModuleImpl::BeginFrame_RenderThread() noexcept
{
	mRenderContext.BeginFrame();

	while (true)
	{
		Thread::YieldUntil(
			[&]() {return mRenderCommandList.HasImmediatelyRenderCommand() || mRenderThreadFrameResourceReadyFlag[GetFrameInfoIndex_RenderThread()]; }
		);

		if (mRenderCommandList.HasImmediatelyRenderCommand())
			mRenderCommandList.ExecuateImmediatelyRenderCommand(mRenderContext);

		if (mRenderThreadFrameResourceReadyFlag[GetFrameInfoIndex_RenderThread()] == true)
		{
			mRenderThreadFrameResourceReadyFlag[GetFrameInfoIndex_RenderThread()] = false;
			break;
		}
	}
}

void SRenderModuleImpl::FrameTick_RenderThread() noexcept
{
	for (RRenderProxyBase* _renderProxy : mRenderCommandList.GetDeferTaskList_RenderThread().mNeedInitRenderProxy)
		_renderProxy->Init(mRenderContext);

	for (RRenderGraphBase* _renderGraph : mRenderCommandList.GetDeferTaskList_RenderThread().mNeedInitRenderGraph)
		_renderGraph->Init(mRenderContext);

	RenderWorld();

	RenderPresentImgui();

	for (RRenderGraphBase* _renderGraph : mRenderCommandList.GetDeferTaskList_RenderThread().mExpiringRenderGraph)
		_renderGraph->Release(mRenderContext);

	for (RRenderProxyBase* _renderProxy : mRenderCommandList.GetDeferTaskList_RenderThread().mExpiringRenderProxy)
		_renderProxy->Release(mRenderContext);

	mRenderCommandList.GetDeferTaskList_RenderThread().OnRenderFrameEnd();
}

void SRenderModuleImpl::EndFrame_RenderThread() noexcept
{
	mRenderContext.EndFrame();

	std::atomic_bool& gameThreadFrameResourceReadyFlag = mGameThreadFrameResourceReadyFlag[GetFrameInfoIndex_RenderThread()];
	++mFrameCount_RenderThread;
	mFrameInfoIndex_RenderThread = mFrameCount_RenderThread % GRenderInfoCount;

	gameThreadFrameResourceReadyFlag = true;
}

void SRenderModuleImpl::RenderThreadMain() noexcept
{
	Thread::SetCurrentThreadName(u8"渲染线程");

	Init_RenderThread();
	while (mRequireExit[GetFrameInfoIndex_RenderThread()] == false)
	{
		BeginFrame_RenderThread();
		FrameTick_RenderThread();
		EndFrame_RenderThread();
	}
	Clear_RenderThread();
}

void SRenderModuleImpl::RenderWorld() noexcept
{
	std::vector<RRenderWorldInfo>& renderWorldList = mRenderCommandList.GetDeferTaskList_RenderThread().mRenderWorldList;

	for (size_t i = 0; i != renderWorldList.size(); ++i)
	{
		RRenderWorldInfo& renderWorldInfo = renderWorldList[i];

		if (renderWorldInfo.mRenderGraph)
			renderWorldInfo.mRenderGraph->Render(renderWorldInfo.mWorld->Get_RenderThread(), renderWorldInfo.mCamera, renderWorldInfo.mCanvas->Get_RenderThread(), mRenderContext);
	}
}

void SRenderModuleImpl::RenderPresentImgui() noexcept
{
	mImguiRenderGraph.Render(mRenderCommandList.GetDeferTaskList_RenderThread().mRenderWindowList, mRenderContext);
}

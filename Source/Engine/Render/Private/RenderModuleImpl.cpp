#include "RDI/RDIModule.h"
#include "Core/Misc/windowsEx.h"
#include "RenderModuleImpl.h"
#include "RDI/Interface/RDIDevice.h"
#include "Core/Modules/ModuleManager.h"
#include "RDI/Interface/RDICommandQueue.h"

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

REGIST_MODULE(L"RenderModule", SRenderModuleImpl)

bool SRenderModuleImpl::Init() noexcept
{
	{
		GRenderModuleImpl = this;
	}

	{
		for (size_t i = 0; i != GRenderInfoCount; ++i)
		{
			mFrameInfoIndex_GameThread = i;
			mFrameInfoIndex_RenderThread = i;
			mFrameResource.Get_GameThread().mGameThreadFrameResourceReadyEvent = CreateEventW(nullptr, FALSE, TRUE, nullptr);
			mFrameResource.Get_RenderThread().mRenderThreadFrameResourceReadyEvent = CreateEventW(nullptr, FALSE, TRUE, nullptr);
		}

		mFrameInfoIndex_GameThread = 0;
		mFrameInfoIndex_RenderThread = 0;
	}

	{
		SModuleManager::Get().LoadModule(L"RDIModule");
		mRdiFactory = GetRDIModule()->GetRDIFactory();
		mRdiDevice = mRdiFactory->GetDevice();
		mRdiCommandQueue = mRdiDevice->GetCommandQueue();
	}

	{
		mRenderThread = std::thread(std::mem_fn(&SRenderModuleImpl::RenderThreadMain), this);
	}

	return true;
}

void SRenderModuleImpl::Clear() noexcept
{
	{
		for (size_t i = 0; i != GRenderInfoCount; ++i)
		{
			mFrameInfoIndex_GameThread = i;
			mFrameInfoIndex_RenderThread = i;
			CloseHandle(mFrameResource.Get_GameThread().mGameThreadFrameResourceReadyEvent);
			CloseHandle(mFrameResource.Get_RenderThread().mRenderThreadFrameResourceReadyEvent);
			mFrameResource.Get_GameThread().mGameThreadFrameResourceReadyEvent = nullptr;
			mFrameResource.Get_RenderThread().mRenderThreadFrameResourceReadyEvent = nullptr;
		}

		mFrameInfoIndex_GameThread = 0;
		mFrameInfoIndex_RenderThread = 0;
	}

	{
		SModuleManager::Get().UnloadModule(L"RDIModule");
	}

	{
		GRenderModuleImpl = nullptr;
	}
}

void SRenderModuleImpl::BeginFrame_GameThread() noexcept
{
	YieldForSingleObject(mFrameResource.Get_GameThread().mGameThreadFrameResourceReadyEvent);
}

void SRenderModuleImpl::EndFrame_GameThread() noexcept
{
	SetEvent(mFrameResource.Get_RenderThread().mRenderThreadFrameResourceReadyEvent);
	++mFrameInfoIndex_GameThread;
	mFrameInfoIndex_GameThread %= GRenderInfoCount;
}

void SRenderModuleImpl::BeginFrame_RenderThread() noexcept
{
	YieldForSingleObject(mFrameResource.Get_RenderThread().mRenderThreadFrameResourceReadyEvent);
	mRdiCommandQueue->YieldUntilCompletion(mFrameResource.Get_RenderThread().mGpuFence);
}

void SRenderModuleImpl::EndFrame_RenderThread() noexcept
{
	SetEvent(mFrameResource.Get_RenderThread().mGameThreadFrameResourceReadyEvent);
	mFrameResource.Get_RenderThread().mGpuFence = mRdiCommandQueue->Signal();
	++mFrameInfoIndex_RenderThread;
	mFrameInfoIndex_RenderThread %= GRenderInfoCount;
}

void SRenderModuleImpl::RenderThreadMain() noexcept
{
	while (mFrameResource.Get_RenderThread().mRequireExit == false)
	{
		BeginFrame_RenderThread();
		FrameTick_RenderThread();
		EndFrame_RenderThread();
	}
}

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
		for (size_t i = 0; i != FrameBufferCount; ++i)
		{
			mGameThreadFrameResourceReadyEvent[i] = CreateEventW(nullptr, FALSE, TRUE, nullptr);
			mRenderThreadFrameResourceReadyEvent[i] = CreateEventW(nullptr, FALSE, FALSE, nullptr);
		}
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
		for (size_t i = 0; i != FrameBufferCount; ++i)
		{
			CloseHandle(mGameThreadFrameResourceReadyEvent[i]);
			CloseHandle(mRenderThreadFrameResourceReadyEvent[i]);
		}
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
	YieldForSingleObject(mGameThreadFrameResourceReadyEvent[mCurrentGameThreadFrameResourceIndex]);
}

void SRenderModuleImpl::EndFrame_GameThread() noexcept
{
	SetEvent(mRenderThreadFrameResourceReadyEvent[mCurrentGameThreadFrameResourceIndex]);
	++mCurrentGameThreadFrameResourceIndex;
	mCurrentGameThreadFrameResourceIndex %= FrameBufferCount;
}

void SRenderModuleImpl::BeginFrame_RenderThread() noexcept
{
	YieldForSingleObject(mRenderThreadFrameResourceReadyEvent[mCurrentRenderThreadFrameResourceIndex]);
	mRdiCommandQueue->YieldUntilCompletion(mGpuFence[mCurrentRenderThreadFrameResourceIndex]);
}

void SRenderModuleImpl::EndFrame_RenderThread() noexcept
{
	SetEvent(mGameThreadFrameResourceReadyEvent[mCurrentRenderThreadFrameResourceIndex]);
	mGpuFence[mCurrentRenderThreadFrameResourceIndex] = mRdiCommandQueue->Signal();
	++mCurrentRenderThreadFrameResourceIndex;
	mCurrentRenderThreadFrameResourceIndex %= FrameBufferCount;
}

void SRenderModuleImpl::RenderThreadMain() noexcept
{
	while (GetFrameResource_RenderThread().mRequireExit == false)
	{
		BeginFrame_RenderThread();
		FrameTick_RenderThread();
		EndFrame_RenderThread();
	}
}

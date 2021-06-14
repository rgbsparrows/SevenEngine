#pragma once

#include "FrameResources.h"
#include "Core/Misc/windowsEx.h"
#include "Render/RenderModule.h"
#include "RDI/Interface/RDIFactory.h"

#include <thread>
#include <string>

class SRenderModuleImpl : public IRenderModule
{
public:
	bool Init() noexcept override;
	void Clear() noexcept override;

	void BeginFrame_GameThread() noexcept override;
	void EndFrame_GameThread() noexcept override;

	void BeginFrame_RenderThread() noexcept;
	void FrameTick_RenderThread() noexcept {}
	void EndFrame_RenderThread() noexcept;

	SFrameResource& GetFrameResource_GameThread() noexcept { return mFrameResource[mCurrentGameThreadFrameResourceIndex]; }
	SFrameResource& GetFrameResource_RenderThread() noexcept { return mFrameResource[mCurrentRenderThreadFrameResourceIndex]; }

private:
	void RenderThreadMain() noexcept;

private:
	static constexpr size_t FrameBufferCount = 3;

	std::thread mRenderThread;

	SFrameResource mFrameResource[FrameBufferCount];

	IRDIFactory* mRdiFactory = nullptr;
	IRDIDevice* mRdiDevice = nullptr;
	IRDICommandQueue* mRdiCommandQueue = nullptr;

	HANDLE mGameThreadFrameResourceReadyEvent[FrameBufferCount] = {};
	HANDLE mRenderThreadFrameResourceReadyEvent[FrameBufferCount] = {};
	uint64_t mGpuFence[FrameBufferCount] = {};

	size_t mCurrentGameThreadFrameResourceIndex = 0;
	size_t mCurrentRenderThreadFrameResourceIndex = 0;
};

SRenderModuleImpl* GetRenderModuleImpl() noexcept;

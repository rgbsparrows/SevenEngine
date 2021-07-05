#pragma once

#include "FrameResources.h"
#include "Core/Misc/windowsEx.h"
#include "Render/RenderModule.h"
#include "RDI/Interface/RDIFactory.h"

#include <thread>
#include <string>
#include "Render/RenderProxy/RenderProxy.h"

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

	size_t GetFrameInfoIndex_GameThread() noexcept override { return mFrameInfoIndex_GameThread; }
	size_t GetFrameInfoIndex_RenderThread() noexcept override { return mFrameInfoIndex_RenderThread; }

private:
	void RenderThreadMain() noexcept;

private:
	std::thread mRenderThread;

	IRDIFactory* mRdiFactory = nullptr;
	IRDIDevice* mRdiDevice = nullptr;
	IRDICommandQueue* mRdiCommandQueue = nullptr;

	size_t mFrameInfoIndex_GameThread = 0;
	size_t mFrameInfoIndex_RenderThread = 0;

	SRenderProxy<SFrameResource, false> mFrameResource;
};

SRenderModuleImpl* GetRenderModuleImpl() noexcept;

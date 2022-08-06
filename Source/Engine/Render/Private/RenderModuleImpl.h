#pragma once

#include "FrameResource.h"
#include "RenderResource.h"
#include "Core/Misc/windowsEx.h"
#include "Render/RenderModule.h"
#include "Render/RenderContent.h"
#include "RenderCommandListImpl.h"
#include "RDI/Interface/RDIFactory.h"
#include "Render/RenderProxy/RenderProxy.h"

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
	void FrameTick_RenderThread() noexcept;
	void EndFrame_RenderThread() noexcept;

	size_t GetFrameInfoIndex_GameThread() noexcept override { return mFrameInfoIndex_GameThread; }
	size_t GetFrameInfoIndex_RenderThread() noexcept override { return mFrameInfoIndex_RenderThread; }

	IRenderCommandList* GetRenderCommandList() noexcept { return &mRenderCommandList; }

private:
	void RenderThreadMain() noexcept;

	void RefrashResources() noexcept;
	void RefrashTextureResource() noexcept;

	void RefrashSwapChain() noexcept;
	void RenderImgui() noexcept;
	void PresentWindows() noexcept;

private:
	IRDICommandAllocator* GetCommandAllocator() noexcept { return mFrameResource->Get_RenderThread().mCommandAllocator; }
	void SyncToGpuFrameEnd(bool _force = false) noexcept;

private:
	std::thread mRenderThread;

	bool mIsSyncToGpuFrameEnd = false;

	IRDIFactory* mRdiFactory = nullptr;
	IRDIDevice* mRdiDevice = nullptr;
	IRDICommandQueue* mRdiCommandQueue = nullptr;
	SRenderContent mMainRenderContent;


	size_t mFrameInfoIndex_GameThread = 0;
	size_t mFrameInfoIndex_RenderThread = 0;

	size_t mFrameCount_GameThread = 0;
	size_t mFrameCount_RenderThread = 0;

	RRenderProxy<RFrameResource>* mFrameResource = nullptr;
	RRenderProxy<RStaticRenderResource>* mStaticRenderResource = nullptr;
	RRenderProxy<RFrameRenderResource>* mFrameRenderResource = nullptr;
	SRenderCommandListImpl mRenderCommandList;
};

SRenderModuleImpl* GetRenderModuleImpl() noexcept;

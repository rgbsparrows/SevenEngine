#pragma once

#include "ImguiRenderGraph.h"
#include "Core/Misc/windowsEx.h"
#include "Render/RenderModule.h"
#include "Render/RenderContext.h"
#include "RenderCommandListImpl.h"
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

	void Init_RenderThread() noexcept;
	void Clear_RenderThread() noexcept;
	void BeginFrame_RenderThread() noexcept;
	void FrameTick_RenderThread() noexcept;
	void EndFrame_RenderThread() noexcept;

	size_t GetFrameInfoIndex_GameThread() noexcept override { return mFrameInfoIndex_GameThread; }
	size_t GetFrameInfoIndex_RenderThread() noexcept override { return mFrameInfoIndex_RenderThread; }

	SRenderContext& GetRenderContext() noexcept { return mRenderContext; }

	IRenderCommandList* GetRenderCommandList() noexcept { return &mRenderCommandList; }

private:
	void RenderThreadMain() noexcept;

	void RenderWorld() noexcept;
	void RenderPresentImgui() noexcept;

private:
	std::thread mRenderThread;
	SRenderContext mRenderContext;

	RImguiRenderGraph mImguiRenderGraph;

	size_t mFrameInfoIndex_GameThread = 0;
	size_t mFrameInfoIndex_RenderThread = 0;

	size_t mFrameCount_GameThread = 0;
	size_t mFrameCount_RenderThread = 0;

	std::atomic_bool mRequireExit[GRenderInfoCount] = { false, false, false };
	std::atomic_bool mGameThreadFrameResourceReadyFlag[GRenderInfoCount] = { true, true, true };
	std::atomic_bool mRenderThreadFrameResourceReadyFlag[GRenderInfoCount] = { false, false, false };

	SRenderCommandListImpl mRenderCommandList;
};

SRenderModuleImpl* GetRenderModuleImpl() noexcept;

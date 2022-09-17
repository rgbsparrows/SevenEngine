#pragma once
#include "Core/Misc/windowsEx.h"
#include "RDI/Interface/RDIDevice.h"
#include "Render/RenderProxy/WindowInfo.h"
#include "Render/RenderProxy/RenderProxy.h"
#include "Render/RenderProxy/World/WorldProxy.h"

#include <functional>

class RWorldRenderGraph;

struct RRenderWorldInfo
{
	RRenderProxy<RWorld>* mWorld = nullptr;
	RRenderProxy<RTexture2D>* mCanvas = nullptr;
	RWorldRenderGraph* mRenderGraph = nullptr;
};

struct RRenderWindowInfo
{
	RRenderProxy<RSwapChain>* mSwapChain = nullptr;
	RRenderProxy<RImguiDrawData>* mImguiDrawData = nullptr;
};

struct RFrameResource
{
public:
	void OnRenderFrameEnd() noexcept
	{
		mNeedInitRenderProxy.resize(0);
		mExpiringRenderProxy.resize(0);
		mRenderWorldList.resize(0);
		mRenderWindowList.resize(0);
	}

public:
	bool mRequireExit = false;
	std::atomic_bool mGameThreadFrameResourceReadyFlag = true;
	std::atomic_bool mRenderThreadFrameResourceReadyFlag = false;
	uint64_t mGpuFence = 0;

	std::vector<RRenderProxyBase*> mNeedInitRenderProxy;
	std::vector<RRenderProxyBase*> mExpiringRenderProxy;

	std::vector<RRenderWorldInfo> mRenderWorldList;
	std::vector<RRenderWindowInfo> mRenderWindowList;
};
template<> struct RRenderInfoTraits<RFrameResource> : RExclusiveMode {};

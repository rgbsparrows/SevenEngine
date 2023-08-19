#pragma once

#include "Render/RenderProxy/WindowInfo.h"
#include "Render/RenderProxy/RenderProxy.h"
#include "Render/RenderProxy/World/WorldProxy.h"

#include <functional>

__interface RRenderGraphBase;

struct RRenderWorldInfo
{
	RRenderProxy<RWorld>* mWorld = nullptr;
	RCamera mCamera;
	RRenderProxy<RTexture2D>* mCanvas = nullptr;
	RRenderGraphBase* mRenderGraph = nullptr;
};

struct RRenderWindowInfo
{
	RRenderProxy<RSwapChain>* mSwapChain = nullptr;
	RRenderProxy<RImguiDrawData>* mImguiDrawData = nullptr;
	bool mEnableVSync = true;
};

struct RDeferFrameTask
{
public:
	void OnRenderFrameEnd() noexcept
	{
		mNeedInitRenderProxy.resize(0);
		mExpiringRenderProxy.resize(0);

		mNeedInitRenderGraph.resize(0);
		mExpiringRenderGraph.resize(0);

		mRenderWorldList.resize(0);
		mRenderWindowList.resize(0);
	}

public:
	std::vector<RRenderProxyBase*> mNeedInitRenderProxy;
	std::vector<RRenderProxyBase*> mExpiringRenderProxy;

	std::vector<RRenderGraphBase*> mNeedInitRenderGraph;
	std::vector<RRenderGraphBase*> mExpiringRenderGraph;

	std::vector<RRenderWorldInfo> mRenderWorldList;
	std::vector<RRenderWindowInfo> mRenderWindowList;
};

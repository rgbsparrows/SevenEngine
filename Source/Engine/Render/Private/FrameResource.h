#pragma once
#include "Core/Misc/windowsEx.h"
#include "RDI/Interface/RDIDevice.h"
#include "Render/RenderProxy/WindowInfo.h"
#include "Render/RenderProxy/RenderProxy.h"

#include <functional>

struct RRefrashSwapChainInfo
{
	RRenderProxy<RSwapChain>* mSwapChain = nullptr;
	RRenderProxy<RSwapChainData>* mSwapChainData = nullptr;
};

struct RRenderWindowInfo
{
	RRenderProxy<RSwapChain>* mSwapChain = nullptr;
	RRenderProxy<RImguiDrawData>* mImguiDrawData = nullptr;
};

struct RRefrashStaticTexture2DInfo
{
	RRenderProxy<RTexture2D>* mTexture2D = nullptr;
	RRenderProxy<RTexture2DData>* mStaticTexture2DDataProxy = nullptr;
	RTexture2DData mStaticTexture2DData;
};

struct RRefrashImTexture2DInfo
{
	RRenderProxy<RTexture2D>* mTexture2D = nullptr;
	RRenderProxy<RImguiTexture2D>* mImTexture = nullptr;
};

struct RFrameResource
{
public:
	void Init(IRDIDevice* _device) noexcept
	{
		mCommandAllocator = _device->CreateCommandAllocator(ERDICommandListType::Direct);
	}

	void OnRenderFrameEnd() noexcept
	{
		mNeedInitRenderProxy.resize(0);
		mExpiringRenderProxy.resize(0);
		mRefrashexture2DList.resize(0);
		mRefrashImTexture2DList.resize(0);
		mRefrashSwapChainList.resize(0);
		mRenderWindowList.resize(0);
	}

public:
	bool mRequireExit = false;
	std::atomic_bool mGameThreadFrameResourceReadyFlag = true;
	std::atomic_bool mRenderThreadFrameResourceReadyFlag = false;
	uint64_t mGpuFence = 0;

	IRDICommandAllocator* mCommandAllocator = nullptr;

	std::vector<RRenderProxyBase*> mNeedInitRenderProxy;
	std::vector<RRenderProxyBase*> mExpiringRenderProxy;

	std::vector<RRefrashStaticTexture2DInfo> mRefrashexture2DList;
	std::vector<RRefrashImTexture2DInfo> mRefrashImTexture2DList;

	std::vector<RRefrashSwapChainInfo> mRefrashSwapChainList;
	std::vector<RRenderWindowInfo> mRenderWindowList;
};
template<> struct RRenderInfoTraits<RFrameResource> : RExclusiveMode {};

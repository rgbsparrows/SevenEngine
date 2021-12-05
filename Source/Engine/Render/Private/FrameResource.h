#pragma once
#include "Core/Misc/windowsEx.h"
#include "Render/RenderProxy/RenderProxy.h"
#include "Render/RenderProxy/WindowInfo.h"

#include <functional>

struct RRefrashSwapChainInfo
{
	RRenderProxy<RSwapChain>* mSwapChain;
	RRenderProxy<RSwapChainData>* mSwapChainData;
};

struct RRenderWindowInfo
{
	RRenderProxy<RSwapChain>* mSwapChain;
	RRenderProxy<RImguiDrawData>* mImguiDrawData;
};

struct RRefrashStaticTexture2DInfo
{
	RRenderProxy<RTexture2D>* mTexture2D;
	RRenderProxy<RTexture2DData>* mStaticTexture2DDataProxy;
	RTexture2DData mStaticTexture2DData;
};

struct RRefrashImTexture2DInfo
{
	RRenderProxy<RTexture2D>* mTexture2D;
	RRenderProxy<RImguiTexture2D>* mImTexture;
};

struct RFrameResource
{
public:
	void Init(IRDIDevice* _device) noexcept
	{
		mGameThreadFrameResourceReadyEvent = CreateEventW(nullptr, FALSE, TRUE, nullptr);
		mRenderThreadFrameResourceReadyEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
	}

	void Clear() noexcept
	{
		CloseHandle(mGameThreadFrameResourceReadyEvent);
		CloseHandle(mRenderThreadFrameResourceReadyEvent);
		mGameThreadFrameResourceReadyEvent = nullptr;
		mRenderThreadFrameResourceReadyEvent = nullptr;
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

	HANDLE mGameThreadFrameResourceReadyEvent = nullptr;
	HANDLE mRenderThreadFrameResourceReadyEvent = nullptr;
	uint64_t mGpuFence = 0;

	std::vector<RRenderProxyBase*> mNeedInitRenderProxy;
	std::vector<RRenderProxyBase*> mExpiringRenderProxy;

	std::vector<RRefrashStaticTexture2DInfo> mRefrashexture2DList;
	std::vector<RRefrashImTexture2DInfo> mRefrashImTexture2DList;

	std::vector<RRefrashSwapChainInfo> mRefrashSwapChainList;
	std::vector<RRenderWindowInfo> mRenderWindowList;
};
template<> struct RRenderInfoTraits<RFrameResource> : RExclusiveMode {};

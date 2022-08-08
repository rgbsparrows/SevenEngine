#pragma once

#include "RenderProxy/World/WorldProxy.h"
#include "Render/RenderProxy/WindowInfo.h"

#include <functional>

//这里的任务并不会严格按照添加/调用顺序来执行
__interface IRenderCommandList
{
	void InitRenderProxy(RRenderProxyBase* _renderProxy) noexcept;
	void InitRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept;

	void AddExpiringRenderProxy(RRenderProxyBase* _renderProxy) noexcept;
	void AddExpiringRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept;

	void RefrashImmediatelyRenderCommand() noexcept;

	void RefrashStaticTexture2D_I(RRenderProxy<RTexture2D>* _texture2D, RTexture2DData&& _textureData) noexcept;
	void RefrashImTexture2D_I(RRenderProxy<RTexture2D>* _texture2D, RRenderProxy<RImguiTexture2D>* _imTexture2D) noexcept;
	void RefrashSwapChain_I(RRenderProxy<RSwapChain>* _swapChain, const RSwapChainData& _swapChainData) noexcept;

	void RenderWorld(RRenderProxy<R3DWorldRenderData>* _3dWorldData, R3DWorldRenderGraph* _renderGraph) noexcept;
	void RenderWindow(RRenderProxy<RSwapChain>* _swapChain, RRenderProxy<RImguiDrawData>* _imguiDrawData) noexcept;
};

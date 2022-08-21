#pragma once

#include "RenderProxy/World/WorldProxy.h"
#include "Render/RenderProxy/WindowInfo.h"

#include <functional>

class RRenderGraphBase;
class R3DWorldRenderGraph;

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
	void ConstructRenderGraph(RRenderGraphBase* _renderGraph) noexcept;

	void RenderWorld(RRenderProxy<R3DWorld>* _3dWorldData, RRenderProxy<RTexture2D>* _canvas, R3DWorldRenderGraph* _renderGraph) noexcept;
	void RenderWindow(RRenderProxy<RSwapChain>* _swapChain, RRenderProxy<RImguiDrawData>* _imguiDrawData) noexcept;
};

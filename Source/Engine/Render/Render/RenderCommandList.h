#pragma once

#include "Render/RenderProxy/WindowInfo.h"

#include <functional>

//这里的任务并不会严格按照添加/调用顺序来执行
__interface IRenderCommandList
{
	void InitRenderProxy(RRenderProxyBase* _renderProxy) noexcept;
	void InitRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept;

	void AddExpiringRenderProxy(RRenderProxyBase* _renderProxy) noexcept;
	void AddExpiringRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept;

	void RefrashStaticTexture2D(RRenderProxy<RTexture2D>* _texture2D, RTexture2DData&& _textureData) noexcept;
	void RefrashImTexture2D(RRenderProxy<RTexture2D>* _texture2D, RRenderProxy<RImguiTexture2D>* _imTexture2D) noexcept;
	void RefrashSwapChain(RRenderProxy<RSwapChain>* _swapChain, RRenderProxy<RSwapChainData>* _swapChainData) noexcept;

	void RefrashImmediatelyRenderCommand() noexcept;

	void RenderWindow(RRenderProxy<RSwapChain>* _swapChain, RRenderProxy<RImguiDrawData>* _imguiDrawData) noexcept;
};

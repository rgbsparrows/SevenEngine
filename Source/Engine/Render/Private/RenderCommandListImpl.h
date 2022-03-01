#pragma once

#include "Render/RenderCommandList.h"
#include "FrameResource.h"

class SRenderCommandListImpl : public IRenderCommandList
{
public:
	void SetFrameResource(RRenderProxy<RFrameResource>* _frameResource) noexcept { mFrameResource = _frameResource; }

	void InitRenderProxy(RRenderProxyBase* _renderProxy) noexcept override;
	void InitRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept override;

	void AddExpiringRenderProxy(RRenderProxyBase* _renderProxy) noexcept override;
	void AddExpiringRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept override;

	void RefrashStaticTexture2D(RRenderProxy<RTexture2D>* _texture2D, RRenderProxy<RTexture2DData>* _textureData) noexcept override;
	void RefrashStaticTexture2D(RRenderProxy<RTexture2D>* _texture2D, RTexture2DData&& _textureData) noexcept override;
	void RefrashImTexture2D(RRenderProxy<RTexture2D>* _texture2D, RRenderProxy<RImguiTexture2D>* _imTexture2D) noexcept override;

	void RefrashSwapChain(RRenderProxy<RSwapChain>* _swapChain, RRenderProxy<RSwapChainData>* _swapChainData) noexcept override;

	void RenderWindow(RRenderProxy<RSwapChain>* _swapChain, RRenderProxy<RImguiDrawData>* _imguiDrawData) noexcept;

private:
	RFrameResource& GetFrameResource_GameThread() noexcept { return mFrameResource->Get_GameThread(); }
	RFrameResource& GetFrameResource_RenderThread() noexcept { return mFrameResource->Get_RenderThread(); }

private:
	RRenderProxy<RFrameResource>* mFrameResource = nullptr;
};

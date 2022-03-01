#include "RenderCommandListImpl.h"

void SRenderCommandListImpl::InitRenderProxy(RRenderProxyBase* _renderProxy) noexcept
{
	GetFrameResource_GameThread().mNeedInitRenderProxy.push_back(_renderProxy);
}

void SRenderCommandListImpl::InitRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept
{
	for (RRenderProxyBase* _renderProxy : _renderProxyList)
		InitRenderProxy(_renderProxy);
}

void SRenderCommandListImpl::AddExpiringRenderProxy(RRenderProxyBase* _renderProxy) noexcept
{
	GetFrameResource_GameThread().mExpiringRenderProxy.push_back(_renderProxy);
}

void SRenderCommandListImpl::AddExpiringRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept
{
	for (RRenderProxyBase* _renderProxy : _renderProxyList)
		AddExpiringRenderProxy(_renderProxy);
}

void SRenderCommandListImpl::RefrashStaticTexture2D(RRenderProxy<RTexture2D>* _texture2D, RRenderProxy<RTexture2DData>* _textureData) noexcept
{
	GetFrameResource_GameThread().mRefrashexture2DList.push_back(RRefrashStaticTexture2DInfo{ _texture2D, _textureData, RTexture2DData() });
}

void SRenderCommandListImpl::RefrashStaticTexture2D(RRenderProxy<RTexture2D>* _texture2D, RTexture2DData&& _textureData) noexcept
{
	GetFrameResource_GameThread().mRefrashexture2DList.push_back(RRefrashStaticTexture2DInfo{ _texture2D, nullptr, std::move(_textureData) });
}

void SRenderCommandListImpl::RefrashImTexture2D(RRenderProxy<RTexture2D>* _texture2D, RRenderProxy<RImguiTexture2D>* _imTexture2D) noexcept
{
	GetFrameResource_GameThread().mRefrashImTexture2DList.push_back(RRefrashImTexture2DInfo{ _texture2D, _imTexture2D });
}

void SRenderCommandListImpl::RefrashSwapChain(RRenderProxy<RSwapChain>* _swapChain, RRenderProxy<RSwapChainData>* _swapChainData) noexcept
{
	GetFrameResource_GameThread().mRefrashSwapChainList.push_back(RRefrashSwapChainInfo{ _swapChain, _swapChainData });
}

void SRenderCommandListImpl::RenderWindow(RRenderProxy<RSwapChain>* _swapChain, RRenderProxy<RImguiDrawData>* _imguiDrawData) noexcept
{
	GetFrameResource_GameThread().mRenderWindowList.push_back(RRenderWindowInfo{ _swapChain, _imguiDrawData });
}

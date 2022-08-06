#include "Core/Misc/Thread.h"
#include "RenderCommandListImpl.h"

bool SRenderCommandListImpl::HasImmediatelyRenderCommand() const noexcept
{
	return mCommandQueueBegin != mCommandQueueEnd;
}

void SRenderCommandListImpl::ExecuateImmediatelyRenderCommand(SRenderContent& _renderContent) noexcept
{
	while (HasImmediatelyRenderCommand())
	{
		mImmediatelyRenderCommandQueue[mCommandQueueBegin](_renderContent);
		mCommandQueueBegin = (mCommandQueueBegin + 1) % mImmediatelyRenderCommandQueue.size();
	}
}

void SRenderCommandListImpl::AddRenderCommand(std::function<void(SRenderContent&)> _renderCommand) noexcept
{
	if (mCommandQueueEnd == (mCommandQueueBegin + 1) % mImmediatelyRenderCommandQueue.size())
	{
		RefrashImmediatelyRenderCommand();
		mImmediatelyRenderCommandQueue.resize(mImmediatelyRenderCommandQueue.size() * 2);
		mCommandQueueBegin = 0;
		mCommandQueueEnd = 0;
	}

	mImmediatelyRenderCommandQueue[mCommandQueueEnd] = _renderCommand;
	mCommandQueueEnd = (mCommandQueueEnd + 1) % mImmediatelyRenderCommandQueue.size();
}

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

void SRenderCommandListImpl::RefrashImmediatelyRenderCommand() noexcept
{
	Thread::YieldUntilValue(mCommandQueueBegin, mCommandQueueEnd.load());
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

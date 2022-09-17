#pragma once

#include "Render/RenderCommandList.h"
#include "FrameResource.h"

class SRenderContent;

class SRenderCommandListImpl : public IRenderCommandList
{
public:
	bool HasImmediatelyRenderCommand() const noexcept;
	void ExecuateImmediatelyRenderCommand(SRenderContent& _renderContent) noexcept;
	void AddRenderCommand(std::function<void(SRenderContent&)> _renderCommand) noexcept;
	
public:
	void SetFrameResource(RRenderProxy<RFrameResource>* _frameResource) noexcept { mFrameResource = _frameResource; }

	void InitRenderProxy(RRenderProxyBase* _renderProxy) noexcept override;
	void InitRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept override;

	void AddExpiringRenderProxy(RRenderProxyBase* _renderProxy) noexcept override;
	void AddExpiringRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept override;

	void RefrashImmediatelyRenderCommand() noexcept override;

	void ConstructRenderGraph(RRenderGraphBase* _renderGraph) noexcept;
	void RefrashStaticTexture2D_I(RRenderProxy<RTexture2D>* _texture2D, RTexture2DData&& _textureData) noexcept override;
	void RefrashImTexture2D_I(RRenderProxy<RTexture2D>* _texture2D, RRenderProxy<RImguiTexture2D>* _imTexture2D) noexcept override;
	void RefrashSwapChain_I(RRenderProxy<RSwapChain>* _swapChain, const RSwapChainData& _swapChainData) noexcept override;
	void RefrashMesh_I(RRenderProxy<RMesh>* _mesh, RMeshData _meshData) noexcept;

	void RenderWorld(RRenderProxy<RWorld>* _3dWorldData, RRenderProxy<RTexture2D>* _canvas, RWorldRenderGraph* _renderGraph) noexcept;
	void RenderWindow(RRenderProxy<RSwapChain>* _swapChain, RRenderProxy<RImguiDrawData>* _imguiDrawData) noexcept;

private:
	RFrameResource& GetFrameResource_GameThread() noexcept { return mFrameResource->Get_GameThread(); }
	RFrameResource& GetFrameResource_RenderThread() noexcept { return mFrameResource->Get_RenderThread(); }

private:
	RRenderProxy<RFrameResource>* mFrameResource = nullptr;

	std::vector<std::function<void(SRenderContent&)>> mImmediatelyRenderCommandQueue;
	std::atomic<size_t> mCommandQueueBegin = 0;
	std::atomic<size_t> mCommandQueueEnd = 0;
};

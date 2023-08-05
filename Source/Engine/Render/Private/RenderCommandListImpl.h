#pragma once

#include "DeferFrameTask.h"
#include "Render/RenderCommandList.h"
#include "Render/RenderGraph/RenderGraphBase.h"

class SRenderContext;

class SRenderCommandListImpl : public IRenderCommandList
{
public:
	bool HasImmediatelyRenderCommand() const noexcept;
	void ExecuateImmediatelyRenderCommand(SRenderContext& _renderContext) noexcept;
	void AddRenderCommand(std::function<void(SRenderContext&)> _renderCommand) noexcept;

	RDeferFrameTask& GetDeferTaskList_GameThread() noexcept { return mDeferFrameTaskList[GetRenderModule()->GetFrameInfoIndex_GameThread()]; }
	RDeferFrameTask& GetDeferTaskList_RenderThread() noexcept { return mDeferFrameTaskList[GetRenderModule()->GetFrameInfoIndex_RenderThread()]; }

public:
	void AddInitRenderProxy(RRenderProxyBase* _renderProxy) noexcept override;
	void AddInitRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept override;

	void AddExpiringRenderProxy(RRenderProxyBase* _renderProxy) noexcept override;
	void AddExpiringRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept override;

	void AddInitRenderGraph(RRenderGraphBase* _renderGraph) noexcept;
	void AddExpiringRenderGraph(RRenderGraphBase* _renderGraph) noexcept;

	void RefrashImmediatelyRenderCommand() noexcept override;

	void RefrashStaticTexture2D_I(RRenderProxy<RTexture2D>* _texture2D, RTexture2DData&& _textureData) noexcept override;
	void RefrashImTexture2D_I(RRenderProxy<RTexture2D>* _texture2D, RRenderProxy<RImguiTexture2D>* _imTexture2D) noexcept override;
	void RefrashSwapChain_I(RRenderProxy<RSwapChain>* _swapChain, const RSwapChainData& _swapChainData) noexcept override;
	void RefrashMesh_I(RRenderProxy<RMesh>* _mesh, RMeshData _meshData) noexcept;

	void RenderWorld_D(RRenderProxy<RWorld>* _world, const RCamera& _camera, RRenderProxy<RTexture2D>* _canvas, RRenderGraphBase* _renderGraph) noexcept;
	void RenderWindow_D(RRenderProxy<RSwapChain>* _swapChain, RRenderProxy<RImguiDrawData>* _imguiDrawData, bool enableVSync) noexcept;

private:
	RDeferFrameTask mDeferFrameTaskList[GRenderInfoCount];

	std::vector<std::function<void(SRenderContext&)>> mImmediatelyRenderCommandQueue;
	std::atomic<size_t> mCommandQueueBegin = 0;
	std::atomic<size_t> mCommandQueueEnd = 0;
};

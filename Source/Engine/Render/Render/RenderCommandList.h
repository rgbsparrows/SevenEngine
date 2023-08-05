#pragma once

#include "RenderProxy/World/WorldProxy.h"
#include "Render/RenderProxy/WindowInfo.h"
#include "RenderProxy/Resource/RenderResource.h"

#include <functional>

__interface RRenderGraphBase;

struct IRenderCommandList
{
	template<CRenderInfo _renderInfoType>
	auto CreateRenderProxy() noexcept
	{
		using RenderInfoType = _renderInfoType;
		RRenderProxy<RenderInfoType>* renderProxy = new RRenderProxy<RenderInfoType>();
		AddInitRenderProxy(renderProxy);
		return renderProxy;
	}

	template<std::derived_from<RRenderGraphBase> _renderGraphType>
	auto CreateRenderGraph() noexcept
	{
		using RenderGraphType = _renderGraphType;
		RenderGraphType* renderGraph = new RenderGraphType();
		AddInitRenderGraph(renderGraph);
		return renderGraph;
	}

	virtual void AddInitRenderProxy(RRenderProxyBase* _renderProxy) noexcept = 0;
	virtual void AddInitRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept = 0;

	virtual void AddExpiringRenderProxy(RRenderProxyBase* _renderProxy) noexcept = 0;
	virtual void AddExpiringRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept = 0;

	virtual void AddInitRenderGraph(RRenderGraphBase* _renderGraph) noexcept = 0;
	virtual void AddExpiringRenderGraph(RRenderGraphBase* _renderGraph) noexcept = 0;

	virtual void RefrashImmediatelyRenderCommand() noexcept = 0;

	virtual void RefrashStaticTexture2D_I(RRenderProxy<RTexture2D>* _texture2D, RTexture2DData&& _textureData) noexcept = 0;
	virtual void RefrashImTexture2D_I(RRenderProxy<RTexture2D>* _texture2D, RRenderProxy<RImguiTexture2D>* _imTexture2D) noexcept = 0;
	virtual void RefrashSwapChain_I(RRenderProxy<RSwapChain>* _swapChain, const RSwapChainData& _swapChainData) noexcept = 0;
	virtual void RefrashMesh_I(RRenderProxy<RMesh>* _mesh, RMeshData _meshData) noexcept = 0;

	virtual void RenderWorld_D(RRenderProxy<RWorld>* _world, const RCamera& _camera, RRenderProxy<RTexture2D>* _canvas, RRenderGraphBase* _renderGraph) noexcept = 0;
	virtual void RenderWindow_D(RRenderProxy<RSwapChain>* _swapChain, RRenderProxy<RImguiDrawData>* _imguiDrawData, bool enableVSync = false) noexcept = 0;
};

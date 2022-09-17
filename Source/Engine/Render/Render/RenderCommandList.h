#pragma once

#include "RenderProxy/World/WorldProxy.h"
#include "Render/RenderProxy/WindowInfo.h"
#include "RenderProxy/Resource/RenderResource.h"

#include <functional>

class RRenderGraphBase;
class RWorldRenderGraph;

struct IRenderCommandList
{
	template<typename _renderInfoType>
	auto CreateRenderProxy() noexcept
	{
		using RenderInfoType = _renderInfoType;
		RRenderProxy<RenderInfoType>* renderProxy = new RRenderProxy<RenderInfoType>();
		InitRenderProxy(renderProxy);
		return renderProxy;
	}

	virtual void InitRenderProxy(RRenderProxyBase* _renderProxy) noexcept = 0;
	virtual void InitRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept = 0;

	virtual void AddExpiringRenderProxy(RRenderProxyBase* _renderProxy) noexcept = 0;
	virtual void AddExpiringRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept = 0;

	virtual void RefrashImmediatelyRenderCommand() noexcept = 0;

	virtual void ConstructRenderGraph(RRenderGraphBase* _renderGraph) noexcept = 0;
	virtual void RefrashStaticTexture2D_I(RRenderProxy<RTexture2D>* _texture2D, RTexture2DData&& _textureData) noexcept = 0;
	virtual void RefrashImTexture2D_I(RRenderProxy<RTexture2D>* _texture2D, RRenderProxy<RImguiTexture2D>* _imTexture2D) noexcept = 0;
	virtual void RefrashSwapChain_I(RRenderProxy<RSwapChain>* _swapChain, const RSwapChainData& _swapChainData) noexcept = 0;
	virtual void RefrashMesh_I(RRenderProxy<RMesh>* _mesh, RMeshData _meshData) noexcept = 0;

	virtual void RenderWorld(RRenderProxy<RWorld>* _3dWorldData, RRenderProxy<RTexture2D>* _canvas, RWorldRenderGraph* _renderGraph) noexcept = 0;
	virtual void RenderWindow(RRenderProxy<RSwapChain>* _swapChain, RRenderProxy<RImguiDrawData>* _imguiDrawData) noexcept = 0;
};

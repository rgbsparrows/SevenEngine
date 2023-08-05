#pragma once

struct RWorld;
struct RCamera;
class SRenderContext;

__interface RRenderGraphBase
{
public:
	void Init(SRenderContext& _renderContext) noexcept;
	void Release(SRenderContext& _renderContext) noexcept;

	void Render(RWorld& _renderData, RCamera& _camera, RTexture2D& _canvas, SRenderContext& _renderGraphBuilder) noexcept;
};

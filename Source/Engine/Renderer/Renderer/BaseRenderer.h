#pragma once

#include "Render/RenderGraph/RenderGraphBase.h"
#include "Render/RenderGraph/ResourceUploader.h"

class RBaseRenderer : public RRenderGraphBase
{
public:
	void Init(SRenderContext& _renderContext) noexcept;
	void Release(SRenderContext& _renderContext) noexcept {}

	void Render(RWorld& _renderData, RCamera& _camera, RTexture2D& _canvas, SRenderContext& _renderContext) noexcept;

private:
	IRDIGraphicsPipelineState* mPipelineState;
	IRDIRootSignature* mRootSignature;
	RDynamicGPUBuffer mDynamicConstantBuffer;

};

#pragma once
#include "DeferFrameTask.h"
#include "Render/RenderGraph/ResourceUploader.h"

class SRenderContext;

class RImguiRenderGraph
{
public:
	void Init(SRenderContext& _renderContext) noexcept;
	void Clear(SRenderContext& _renderContext) noexcept;

	void Render(std::vector<RRenderWindowInfo>& _renderWindowInfo, SRenderContext& _renderContext) noexcept;

private:
	IRDIGraphicsPipelineState* mImguiSDRPipelineState = nullptr;
	IRDIGraphicsPipelineState* mImguiHDR10PipelineState = nullptr;
	IRDIGraphicsPipelineState* mImguiHDR1000PipelineState = nullptr;

	RDynamicGPUBuffer mDynamicConstantBuffer;
	std::vector<RDynamicGPUBuffer> mDynamicIndexBufferList;
	std::vector<RDynamicGPUBuffer> mDynamicVertexBufferList;
};

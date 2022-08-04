#pragma once

#include "FrameResource.h"
#include "ResourceUploader.h"
#include "Render/RenderModule.h"

#include <vector>

class RImguiRenderPass
{
public:
	void Setup() noexcept;
	void Clear() noexcept;

	void PreRender(std::vector<RRenderWindowInfo>& _renderWindowList) noexcept;
	void Render(std::vector<RRenderWindowInfo>& _renderWindowList) noexcept;
	void PostRender(std::vector<RRenderWindowInfo>& _renderWindowList) noexcept;

private:
	IRDIGraphicsPipelineState* mImguiSDRPipelineState = nullptr;
	IRDIGraphicsPipelineState* mImguiHDR10PipelineState = nullptr;
	IRDIGraphicsPipelineState* mImguiHDR1000PipelineState = nullptr;
	IRDIRootSignature* mImguiRootSignature = nullptr;

	RDynamicGPUBuffer mImguiConstantBuffer;
};

#pragma once
#include "Render/RenderProxy/RenderProxy.h"
#include "Core/Util/UtilMacros.h"

__interface IRDIDevice;
__interface IRDIBuffer;
__interface IRDIDescriptorHeapRange;
__interface IRDIGraphicsPipelineState;
__interface IRDIRootSignature;

struct RStaticRenderResource
{
public:
	void Init(IRDIDevice* _device) noexcept;
	void Clear() noexcept;

public:
	IRDIGraphicsPipelineState* mImguiSDRPipelineState = nullptr;
	IRDIGraphicsPipelineState* mImguiHDR10PipelineState = nullptr;
	IRDIGraphicsPipelineState* mImguiHDR1000PipelineState = nullptr;
	IRDIRootSignature* mImguiRootSignature = nullptr;
	IRDIBuffer* mConstantBuffer = nullptr;
};
template<> struct RRenderInfoTraits<RStaticRenderResource> : RShareMode {};

struct RFrameRenderResource
{
public:
	void Clear() noexcept;

public:
	bool mEnableVSync = false;
	IRDIBuffer* mConstantUploadBuffer = nullptr;
};
template<> struct RRenderInfoTraits<RFrameRenderResource> : RExclusiveMode {};

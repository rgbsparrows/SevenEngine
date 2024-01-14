#pragma once

#include "Core/Container/Blob.h"

#include <stdint.h>

struct SRDIBufferResourceDesc;
struct SRDITexture1DResourceDesc;
struct SRDITexture1DArrayResourceDesc;
struct SRDITexture2DResourceDesc;
struct SRDITexture2DArrayResourceDesc;
struct SRDITexture3DResourceDesc;
struct SRDITextureCubeResourceDesc;
struct SRDITextureCubeArrayResourceDesc;
struct SRDISamplerResourceDesc;

struct SRDISwapChainDesc;
struct SRDIGraphicsPipelineStateDesc;
struct SRDIComputePipelineStateDesc;

struct SRDIShaderMacro;
struct SRDIAdditionalInclude;
struct SRDIErrorInfo;

enum class ERDICommandListType;

__interface IRDIAdapter;
__interface IRDISwapChain;
__interface IRDICommandList;
__interface IRDICommandQueue;
__interface IRDICommandAllocator;
__interface IRDISamplerHeapRange;
__interface IRDIDescriptorHeapRange;
__interface IRDIComputePipelineState;
__interface IRDIGraphicsPipelineState;

__interface IRDIBuffer;
__interface IRDITexture1D;
__interface IRDITexture1DArray;
__interface IRDITexture2D;
__interface IRDITexture2DArray;
__interface IRDITexture3D;
__interface IRDITextureCube;
__interface IRDITextureCubeArray;
__interface IRDISampler;

__interface IRDIDevice
{
	IRDIAdapter* GetAdapter() noexcept;
	IRDICommandQueue* GetCommandQueue() noexcept;
	IRDICommandQueue* GetComputeCommandQueue() noexcept;

	IRDICommandAllocator* CreateCommandAllocator(ERDICommandListType _commandListType) noexcept;
	IRDICommandList* CreateCommandList(ERDICommandListType _commandListType, IRDICommandAllocator* _commandAllocator) noexcept;

	IRDISwapChain* CreateSwapChain(const SRDISwapChainDesc* _swapChainDesc) noexcept;

	IRDIGraphicsPipelineState* CreateGraphicsPipelineState(const SRDIGraphicsPipelineStateDesc* _desc) noexcept;
	IRDIComputePipelineState* CreateComputePipelineState(const SRDIComputePipelineStateDesc* _desc) noexcept;

	IRDIBuffer* CreateBuffer(const SRDIBufferResourceDesc* _desc) noexcept;
	IRDITexture1D* CreateTexture1D(const SRDITexture1DResourceDesc* _desc) noexcept;
	IRDITexture1DArray* CreateTexture1DArray(const SRDITexture1DArrayResourceDesc* _desc) noexcept;
	IRDITexture2D* CreateTexture2D(const SRDITexture2DResourceDesc* _desc) noexcept;
	IRDITexture2DArray* CreateTexture2DArray(const SRDITexture2DArrayResourceDesc* _desc) noexcept;
	IRDITexture3D* CreateTexture3D(const SRDITexture3DResourceDesc* _desc) noexcept;
	IRDITextureCube* CreateTextureCube(const SRDITextureCubeResourceDesc* _desc) noexcept;
	IRDITextureCubeArray* CreateTextureCubeArray(const SRDITextureCubeArrayResourceDesc* _desc) noexcept;
	IRDISampler* CreateSampler(const SRDISamplerResourceDesc* _desc) noexcept;

	IRDIDescriptorHeapRange* CreateDescriptorRange(uint16_t _srvCount, uint16_t _uavCount) noexcept;
	IRDISamplerHeapRange* CreateDescriptorRange(uint16_t _samplerCount) noexcept;
};

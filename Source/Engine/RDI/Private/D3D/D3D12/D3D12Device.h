#pragma once

#include "RDI/Interface/RDIDevice.h"

#include "D3D/Helper/D3DHelper.h"
#include "D3D/D3D12/D3D12Adapter.h"
#include "D3D/D3D12/D3D12SwapChain.h"
#include "D3D/D3D12/D3D12CommandList.h"
#include "D3D/D3D12/D3D12CommandQueue.h"
#include "D3D/D3D12/D3D12PipelineState.h"
#include "D3D/D3D12/D3D12DescriptorHeap.h"
#include "D3D/D3D12/D3D12CommandAllocator.h"

#include "Core/Misc/PreWindowsApi.h"
#include <d3d12.h>
#include <d3dcompiler.h>
#include "Core/Misc/PostWindowsApi.h"

#include <vector>
#include <functional>

class SD3D12Factory;

class SD3D12Device : public IRDIDevice
{
public:
	void Init(ID3D12Device* _nativePtr, SD3D12Adapter* _adapter, SD3D12Factory* _factory) noexcept;
	void Clear() noexcept;

	ID3D12Device* GetNativePtr() noexcept { return mD3D12DeviceNativePtr; }

public:
	IRDIAdapter* GetAdapter() noexcept override { return mAdapter; }
	IRDICommandQueue* GetCommandQueue() noexcept override { return &mCommandQueue; }
	IRDICommandQueue* GetComputeCommandQueue() noexcept { return &mComputeCommandQueue; }

	IRDICommandAllocator* CreateCommandAllocator(ERDICommandListType _commandListType) noexcept;
	IRDICommandList* CreateCommandList(ERDICommandListType _commandListType, IRDICommandAllocator* _commandAllocator) noexcept;

	IRDISwapChain* CreateSwapChain(const SRDISwapChainDesc* _swapChainDesc) noexcept override;

	IRDIGraphicsPipelineState* CreateGraphicsPipelineState(const SRDIGraphicsPipelineStateDesc* _desc) noexcept override;
	IRDIComputePipelineState* CreateComputePipelineState(const SRDIComputePipelineStateDesc* _desc) noexcept override;

	IRDIBuffer* CreateBuffer(const SRDIBufferResourceDesc* _desc) noexcept override;
	IRDITexture1D* CreateTexture1D(const SRDITexture1DResourceDesc* _desc) noexcept override;
	IRDITexture1DArray* CreateTexture1DArray(const SRDITexture1DArrayResourceDesc* _desc) noexcept override;
	IRDITexture2D* CreateTexture2D(const SRDITexture2DResourceDesc* _desc) noexcept override;
	IRDITexture2DArray* CreateTexture2DArray(const SRDITexture2DArrayResourceDesc* _desc) noexcept override;
	IRDITexture3D* CreateTexture3D(const SRDITexture3DResourceDesc* _desc) noexcept override;
	IRDITextureCube* CreateTextureCube(const SRDITextureCubeResourceDesc* _desc) noexcept override;
	IRDITextureCubeArray* CreateTextureCubeArray(const SRDITextureCubeArrayResourceDesc* _desc) noexcept override;
	IRDISampler* CreateSampler(const SRDISamplerResourceDesc* _desc) noexcept override;

	IRDIDescriptorHeapRange* CreateDescriptorRange(uint16_t _srvCount, uint16_t _uavCount) noexcept;
	IRDISamplerHeapRange* CreateDescriptorRange(uint16_t _samplerCount) noexcept;

public:
	void ReleaseCommandAllocator(SD3D12CommandAllocator* _commandAllocator) noexcept;
	void ReleaseCommandList(SD3D12CommandList* _commandList) noexcept;

	void ReleaseSwapChain(SD3D12SwapChain* _swapChain) noexcept;

	void ReleaseGraphicsPipelineState(SD3D12GraphicsPipelineState* _graphicPipelineState) noexcept;
	void ReleaseComputePipelineState(SD3D12ComputePipelineState* _computePipelineState) noexcept;

	void ReleaseBuffer(SD3D12Buffer* _buffer) noexcept;
	void ReleaseTexture1D(SD3D12Texture1D* _texture1D) noexcept;
	void ReleaseTexture1DArray(SD3D12Texture1DArray* _texture1DArray) noexcept;
	void ReleaseTexture2D(SD3D12Texture2D* _texture2D) noexcept;
	void ReleaseTexture2DArray(SD3D12Texture2DArray* _texture2DArray) noexcept;
	void ReleaseTexture3D(SD3D12Texture3D* _texture3D) noexcept;
	void ReleaseTextureCube(SD3D12TextureCube* _textureCube) noexcept;
	void ReleaseTextureCubeArray(SD3D12TextureCubeArray* _textureCubeArray) noexcept;
	void ReleaseSampler(SD3D12Sampler* _sampler) noexcept;

public:
	SD3D12DescriptorHeap* GetDescriptorHeap() noexcept { return &mDescriptorHeap; }
	SD3D12ShaderVisibleDescriptorHeap* GetShaderVisibleDescriptorHeap() noexcept { return &mShaderVisibleDescriptorHeap; }
	uint32_t GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE _descriptorHeapType) noexcept { return mDescriptorHandleIncrement[EnumToInt(_descriptorHeapType)]; }
	IRDITexture2D* CreateTexture2DWithCreatedResource(const SRDITexture2DResourceDesc* _desc, ID3D12Resource* _resource) noexcept;

public:
	D3D12_RESOURCE_FLAGS MakeD3D12ResourceFlag(ERDIResourceUsage _resourceUsage) noexcept;
	D3D12_CLEAR_VALUE MakeD3D12ClearValue(ERDIResourceUsage _resourceUsage, ERDIPixelFormat _pixelFormat, ERDIPixelFormat _rtvPixelFormat, ERDIPixelFormat _dsvPixelFormat, Math::SFColor _clearColor, float _clearDepth, uint8_t _clearStencil) noexcept;
	ID3D12Resource* CreateCommittedResource(ERDIHeapType _heapType, const D3D12_RESOURCE_DESC* _desc, const D3D12_CLEAR_VALUE* _clearValue) noexcept;
	SBlob CreateRootSignatureBlob(const SRDIRootSignature* _rootSignature) noexcept;

private:
	ID3D12Device* mD3D12DeviceNativePtr = nullptr;
	SD3D12Factory* mFactory = nullptr;

	static constexpr uint64_t ShaderVisibleCbvSrvUavDescriptorCount = 4096;
	static constexpr uint64_t ShaderVisibleSamplerViewDescriptorCount = 512;
	static constexpr uint64_t RTVDescriptorCount = 512;
	static constexpr uint64_t DSVDescriptorCount = 512;
	static constexpr uint64_t SRVDescriptorCount = 2048;
	static constexpr uint64_t UAVDescriptorCount = 2048;
	static constexpr uint64_t SamplerViewDescriptorCount = 4096;

	SD3D12Adapter* mAdapter = nullptr;
	SD3D12CommandQueue mCommandQueue;
	SD3D12CommandQueue mComputeCommandQueue;

	SD3D12ShaderVisibleDescriptorHeap mShaderVisibleDescriptorHeap;
	SD3D12DescriptorHeap mDescriptorHeap;

	TElementPool<SD3D12CommandAllocator> mCommandAllocatorPool;
	TElementPool<SD3D12CommandList> mCommandListPool;

	TElementPool<SD3D12SwapChain> mSwapChainPool;

	TElementPool<SD3D12GraphicsPipelineState> mGraphicsPipelineStatePool;
	TElementPool<SD3D12ComputePipelineState> mComputePipelineStatePool;

	TElementPool<SD3D12Buffer> mBufferPool;
	TElementPool<SD3D12Texture1D> mTexture1DPool;
	TElementPool<SD3D12Texture1DArray> mTexture1DArrayPool;
	TElementPool<SD3D12Texture2D> mTexture2DPool;
	TElementPool<SD3D12Texture2DArray> mTexture2DArrayPool;
	TElementPool<SD3D12Texture3D> mTexture3DPool;
	TElementPool<SD3D12TextureCube> mTextureCubePool;
	TElementPool<SD3D12TextureCubeArray> mTextureCubeArrayPool;
	TElementPool<SD3D12Sampler> mSamplerPool;

	uint32_t mDescriptorHandleIncrement[EnumToInt(D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES)] = {};
};

#pragma once

#include "D3D/D3D12/D3D12Resource.h"
#include "Core/Container/ElementPool.h"
#include "Core/Allocator/AllocatorHelper.h"
#include "RDI/Interface/RDIDescriptorHeapRange.h"

#include <stdint.h>

__interface IRDIRenderTargetView;
__interface IRDIDepthStencilView;
__interface IRDIShaderResourceView;
__interface IRDIUnorderedAccessView;
__interface IRDISamplerView;

__interface IRDIBuffer;
__interface IRDITexture1D;
__interface IRDITexture1DArray;
__interface IRDITexture2D;
__interface IRDITexture2DArray;
__interface IRDITexture3D;
__interface IRDITextureCube;
__interface IRDITextureCubeArray;
__interface IRDISampler;

__interface IRDIDescriptorHeapRange;

enum class ERDITextureCubeFace;

class SD3D12Device;
class SD3D12ShaderVisibleDescriptorHeap;

class SD3D12DescriptorHeapRange : public IRDIDescriptorHeapRange
{
public:
	void Init(uint64_t _srvUavDescriptorHeapIndex, uint16_t _srvDescriptorCount, uint16_t _uavDescriptorCount, SD3D12ShaderVisibleDescriptorHeap* _shaderVisibleDescriptorHeap) noexcept;
	uint64_t GetSrvUavDescriptorHeapIndex() const noexcept { return mSrvUavDescriptorHeapIndex; }
	uint64_t GetSrvDescriptorHeapIndex() const noexcept { return mSrvUavDescriptorHeapIndex; }
	uint64_t GetUavDescriptorHeapIndex() const noexcept { return mSrvUavDescriptorHeapIndex + mSrvDescriptorCount; }
	uint16_t GetSrvDescriptorCount() const noexcept { return mSrvDescriptorCount; }
	uint16_t GetUavDescriptorCount() const noexcept { return mUavDescriptorCount; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuDescriptorHandle() const noexcept { return mCPUDescriptorHandle; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuDescriptorHandle() const noexcept { return mGPUDescriptorHandle; }

public:
	void SetSRV(IRDIShaderResourceView* _srv, uint64_t _offset) noexcept override;
	void SetUAV(IRDIUnorderedAccessView* _uav, uint64_t _offset) noexcept override;
	void Release() noexcept override;

private:
	SD3D12ShaderVisibleDescriptorHeap* mShaderVisibleDescriptorHeap = nullptr;

	uint64_t mSrvUavDescriptorHeapIndex = 0;
	uint16_t mSrvDescriptorCount = 0;
	uint16_t mUavDescriptorCount = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE mCPUDescriptorHandle = {};
	D3D12_GPU_DESCRIPTOR_HANDLE mGPUDescriptorHandle = {};
};

class SD3D12SamplerHeapRange : public IRDISamplerHeapRange
{
public:
	void Init(uint64_t _samplerViewDescriptorHeapIndex, uint16_t _samplerViewDescriptorCount, SD3D12ShaderVisibleDescriptorHeap* _shaderVisibleDescriptorHeap) noexcept;
	uint64_t GetSamplerViewDescriptorHeapIndex() const noexcept { return mSamplerViewDescriptorHeapIndex; }
	uint64_t GetSamplerViewDescriptorCount() const noexcept { return mSamplerViewDescriptorCount; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuDescriptorHandle() const noexcept { return mCPUDescriptorHandle; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuDescriptorHandle() const noexcept { return mGPUDescriptorHandle; }

public:
	void SetSampler(IRDISamplerView* _sampler, uint64_t _offset) noexcept override;
	void Release() noexcept override;

private:
	SD3D12ShaderVisibleDescriptorHeap* mShaderVisibleDescriptorHeap = nullptr;

	uint64_t mSamplerViewDescriptorHeapIndex = 0;
	uint16_t mSamplerViewDescriptorCount = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE mCPUDescriptorHandle = {};
	D3D12_GPU_DESCRIPTOR_HANDLE mGPUDescriptorHandle = {};
};

class SD3D12ShaderVisibleDescriptorHeap
{
public:
	void Init(
		ID3D12DescriptorHeap* _srvUavDescriptorHeapNativePtr,
		uint64_t _srvUavDescriptorCount,
		ID3D12DescriptorHeap* _samplerViewDescriptorHeapNativePtr,
		uint64_t _samplerViewDescriptorCount,
		SD3D12Device* _device
	) noexcept;
	void Clear() noexcept;

	ID3D12DescriptorHeap* GetSrvUavDescriptorHeapNativePtr() noexcept { return mSrvUavDescriptorHeapNativePtr; }
	ID3D12DescriptorHeap* GetSamplerViewDescriptorHeapNativePtr() noexcept { return mSamplerViewDescriptorHeapNativePtr; }

public:
	SD3D12DescriptorHeapRange* AllocateDescriptorHeapRange(uint16_t _srvDescriptorCount, uint16_t _uavDescriptorCount) noexcept;
	SD3D12SamplerHeapRange* AllocateSamplerHeapRange(uint16_t _samplerViewDescriptorCount) noexcept;
	void ReleaseDescriptorHeapRange(IRDIDescriptorHeapRange* _descriptorHeapRange) noexcept;
	void ReleaseSamplerHeapRange(IRDISamplerHeapRange* _samplerHeapRange) noexcept;

	//下面这些Set方法应该都只通过IRDIDescriptorHeapRange中的Set方法间接调用
	void SetSRV(IRDIShaderResourceView* _srv, uint64_t _index) noexcept;
	void SetUAV(IRDIUnorderedAccessView* _uav, uint64_t _index) noexcept;
	void SetSampler(IRDISamplerView* _sampler, uint64_t _index) noexcept;

	uint32_t GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE _descriptorHeapType) noexcept;

	D3D12_CPU_DESCRIPTOR_HANDLE GetSrvUavDescriptorHeapStartCPUDescriptorHandle() noexcept { return mSrvUavDescriptorHeapStartCPUDescriptorHandle; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetSamplerViewDescriptorHeapStartCPUDescriptorHandle() noexcept { return mSamplerViewDescriptorHeapStartCPUDescriptorHandle; }

	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvUavDescriptorHeapStartGPUDescriptorHandle() noexcept { return mSrvUavDescriptorHeapStartGPUDescriptorHandle; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetSamplerViewDescriptorHeapStartGPUDescriptorHandle() noexcept { return mSamplerViewDescriptorHeapStartGPUDescriptorHandle; }

private:
	SD3D12Device* mDevice = nullptr;

	ID3D12DescriptorHeap* mSrvUavDescriptorHeapNativePtr = nullptr;
	ID3D12DescriptorHeap* mSamplerViewDescriptorHeapNativePtr = nullptr;

	SCircularSlotArrayAllocatorHelper mSrvUavDescriptorAlloctorHelper;
	SCircularSlotArrayAllocatorHelper mSamplerViewAlloctorHelper;

	TElementPool<SD3D12DescriptorHeapRange> mDescriptorHeapRangePool;
	TElementPool<SD3D12SamplerHeapRange> mSamplerHeapRangePool;

	D3D12_CPU_DESCRIPTOR_HANDLE mSrvUavDescriptorHeapStartCPUDescriptorHandle = {};
	D3D12_CPU_DESCRIPTOR_HANDLE mSamplerViewDescriptorHeapStartCPUDescriptorHandle = {};

	D3D12_GPU_DESCRIPTOR_HANDLE mSrvUavDescriptorHeapStartGPUDescriptorHandle = {};
	D3D12_GPU_DESCRIPTOR_HANDLE mSamplerViewDescriptorHeapStartGPUDescriptorHandle = {};
};

class SD3D12DescriptorHeap
{
public:
	void Init(
		ID3D12DescriptorHeap* _rtvDescriptorHeapNativePtr,
		uint64_t _rtvDescriptorCount,
		ID3D12DescriptorHeap* _dsvDescriptorHeapNativePtr,
		uint64_t _dsvDescriptorCount,
		ID3D12DescriptorHeap* _srvDescriptorHeapNativePtr,
		uint64_t _srvDescriptorCount,
		ID3D12DescriptorHeap* _uavDescriptorHeapNativePtr,
		uint64_t _uavDescriptorCount,
		ID3D12DescriptorHeap* _samplerViewDescriptorHeapNativePtr,
		uint64_t _samplerViewDescriptorCount,
		SD3D12Device* _device
	) noexcept;
	void Clear() noexcept;

public:
	SD3D12RenderTargetView* CreateRTV(IRDITexture1D* _resource, uint32_t _mipSlice) noexcept;
	SD3D12RenderTargetView* CreateRTV(IRDITexture2D* _resource, uint32_t _mipSlice) noexcept;
	SD3D12RenderTargetView* CreateRTV(IRDITexture3D* _resource, uint32_t _mipSlice) noexcept;
	SD3D12RenderTargetView* CreateRTV(IRDITextureCube* _resource, ERDITextureCubeFace _cubeFace, uint32_t _mipSlice) noexcept;

	SD3D12DepthStencilView* CreateDSV(IRDITexture1D* _resource, uint32_t _mipSlice) noexcept;
	SD3D12DepthStencilView* CreateDSV(IRDITexture2D* _resource, uint32_t _mipSlice) noexcept;
	SD3D12DepthStencilView* CreateDSV(IRDITextureCube* _resource, ERDITextureCubeFace _cubeFace, uint32_t _mipSlice) noexcept;

	SD3D12ShaderResourceView* CreateSRV(IRDIBuffer* _resource) noexcept;
	SD3D12ShaderResourceView* CreateSRV(IRDITexture1D* _resource) noexcept;
	SD3D12ShaderResourceView* CreateSRV(IRDITexture1DArray* _resource) noexcept;
	SD3D12ShaderResourceView* CreateSRV(IRDITexture2D* _resource) noexcept;
	SD3D12ShaderResourceView* CreateSRV(IRDITexture2DArray* _resource) noexcept;
	SD3D12ShaderResourceView* CreateSRV(IRDITexture3D* _resource) noexcept;
	SD3D12ShaderResourceView* CreateSRV(IRDITextureCube* _resource) noexcept;
	SD3D12ShaderResourceView* CreateSRV(IRDITextureCubeArray* _resource) noexcept;

	SD3D12UnorderedAccessView* CreateUAV(IRDIBuffer* _resource) noexcept;
	SD3D12UnorderedAccessView* CreateUAV(IRDITexture1D* _resource, uint32_t _mipSlice) noexcept;
	SD3D12UnorderedAccessView* CreateUAV(IRDITexture1DArray* _resource, uint32_t _mipSlice) noexcept;
	SD3D12UnorderedAccessView* CreateUAV(IRDITexture2D* _resource, uint32_t _mipSlice) noexcept;
	SD3D12UnorderedAccessView* CreateUAV(IRDITexture2DArray* _resource, uint32_t _mipSlice) noexcept;
	SD3D12UnorderedAccessView* CreateUAV(IRDITexture3D* _resource, uint32_t _mipSlice) noexcept;
	SD3D12UnorderedAccessView* CreateUAV(IRDITextureCube* _resource, uint32_t _mipSlice) noexcept;
	SD3D12UnorderedAccessView* CreateUAV(IRDITextureCubeArray* _resource, uint32_t _mipSlice) noexcept;

	SD3D12SamplerView* CreateSamplerView(IRDISampler* _sampler) noexcept;

	void ReleaseRTV(SD3D12RenderTargetView* _rtv) noexcept;
	void ReleaseDSV(SD3D12DepthStencilView* _dsv) noexcept;
	void ReleaseSRV(SD3D12ShaderResourceView* _srv) noexcept;
	void ReleaseUAV(SD3D12UnorderedAccessView* _uav) noexcept;
	void ReleaseSamplerView(SD3D12SamplerView* _samplerView) noexcept;

private:
	SD3D12RenderTargetView* CreateRTV(ID3D12Resource* _resourceNativePtr, const D3D12_RENDER_TARGET_VIEW_DESC* _desc) noexcept;
	SD3D12DepthStencilView* CreateDSV(ID3D12Resource* _resourceNativePtr, const D3D12_DEPTH_STENCIL_VIEW_DESC* _desc) noexcept;
	SD3D12ShaderResourceView* CreateSRV(ID3D12Resource* _resourceNativePtr, const D3D12_SHADER_RESOURCE_VIEW_DESC* _desc) noexcept;
	SD3D12UnorderedAccessView* CreateUAV(ID3D12Resource* _resourceNativePtr, const D3D12_UNORDERED_ACCESS_VIEW_DESC* _desc) noexcept;
	SD3D12SamplerView* CreateSamplerView(const D3D12_SAMPLER_DESC* _desc) noexcept;

private:
	SD3D12Device* mDevice = nullptr;

	ID3D12DescriptorHeap* mRTVDescriptorHeapNativePtr = nullptr;
	ID3D12DescriptorHeap* mDSVDescriptorHeapNativePtr = nullptr;
	ID3D12DescriptorHeap* mSRVDescriptorHeapNativePtr = nullptr;
	ID3D12DescriptorHeap* mUAVDescriptorHeapNativePtr = nullptr;
	ID3D12DescriptorHeap* mSamplerViewDescriptorHeapNativePtr = nullptr;

	SCircularSlotAllocatorHelper mRTVAllocatorHelper;
	SCircularSlotAllocatorHelper mDSVAllocatorHelper;
	SCircularSlotAllocatorHelper mSRVAllocatorHelper;
	SCircularSlotAllocatorHelper mUAVAllocatorHelper;
	SCircularSlotAllocatorHelper mSamplerViewAllocatorHelper;

	TElementPool<SD3D12RenderTargetView> mRenderTargetViewPool;
	TElementPool<SD3D12DepthStencilView> mDepthStencilViewPool;
	TElementPool<SD3D12ShaderResourceView> mShaderResourceViewPool;
	TElementPool<SD3D12UnorderedAccessView> mUnorderedAccessViewPool;
	TElementPool<SD3D12SamplerView> mSamplerViewPool;

	D3D12_CPU_DESCRIPTOR_HANDLE mRTVDescriptorHeapStartCPUDescriptorHandle = {};
	D3D12_CPU_DESCRIPTOR_HANDLE mDSVDescriptorHeapStartCPUDescriptorHandle = {};
	D3D12_CPU_DESCRIPTOR_HANDLE mSRVDescriptorHeapStartCPUDescriptorHandle = {};
	D3D12_CPU_DESCRIPTOR_HANDLE mUAVDescriptorHeapStartCPUDescriptorHandle = {};
	D3D12_CPU_DESCRIPTOR_HANDLE mSamplerViewDescriptorHeapStartCPUDescriptorHandle = {};

	D3D12_GPU_DESCRIPTOR_HANDLE mRTVDescriptorHeapStartGPUDescriptorHandle = {};
	D3D12_GPU_DESCRIPTOR_HANDLE mDSVDescriptorHeapStartGPUDescriptorHandle = {};
	D3D12_GPU_DESCRIPTOR_HANDLE mSRVDescriptorHeapStartGPUDescriptorHandle = {};
	D3D12_GPU_DESCRIPTOR_HANDLE mUAVDescriptorHeapStartGPUDescriptorHandle = {};
	D3D12_GPU_DESCRIPTOR_HANDLE mSamplerViewDescriptorHeapStartGPUDescriptorHandle = {};
};

﻿#pragma once

#include "RDI/RDIFunctionHelper.h"
#include "RDI/Interface/RDIResource.h"
#include "D3D/D3D12/Helper/D3D12Helper.h"

#include "Core/Misc/PreWindowsApi.h"
#include <d3d12.h>
#include "Core/Misc/PostWindowsApi.h"

#include <limits>

class SD3D12Device;
class SD3D12SamplerView;

class SD3D12ViewBase
{
public:
	SD3D12ViewBase(D3D12_CPU_DESCRIPTOR_HANDLE _cpuDescriptorHandle) :
		mCpuDescriptorHandle(_cpuDescriptorHandle)
	{
	}

	D3D12_CPU_DESCRIPTOR_HANDLE mCpuDescriptorHandle = {};
};

class SD3D12IndexBufferView : public IRDIIndexBufferView
{
public:
	D3D12_GPU_VIRTUAL_ADDRESS mGpuVirtualAddress = 0;
	uint32_t mSizeInByte = 0;
	DXGI_FORMAT mFormat = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
};

class SD3D12VertexBufferView : public IRDIVertexBufferView
{
public:
	D3D12_GPU_VIRTUAL_ADDRESS mGpuVirtualAddress = 0;
	uint32_t mSizeInByte = 0;
	uint16_t mStrideInBytes = 0;
};

class SD3D12RenderTargetView : public IRDIRenderTargetView, public SD3D12ViewBase
{
public:
	SD3D12RenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE _cpuDescriptorHandle, Math::SFColor _clearColor)
		:SD3D12ViewBase(_cpuDescriptorHandle), mClearColor(_clearColor)
	{
	}

	Math::SFColor mClearColor;
};

class SD3D12DepthStencilView : public IRDIDepthStencilView, public SD3D12ViewBase
{
public:
	SD3D12DepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE _cpuDescriptorHandle, float _clearDepth, uint8_t _clearStencil)
		:SD3D12ViewBase(_cpuDescriptorHandle), mClearDepth(_clearDepth), mClearStencil(_clearStencil)
	{
	}

	float mClearDepth = 1.f;
	uint8_t mClearStencil = 0;
};

class SD3D12ShaderResourceView : public IRDIShaderResourceView, public SD3D12ViewBase
{
public:
	SD3D12ShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE _cpuDescriptorHandle)
		:SD3D12ViewBase(_cpuDescriptorHandle)
	{
	}
};

class SD3D12UnorderedAccessView : public IRDIUnorderedAccessView, public SD3D12ViewBase
{
public:
	SD3D12UnorderedAccessView(D3D12_CPU_DESCRIPTOR_HANDLE _cpuDescriptorHandle)
		:SD3D12ViewBase(_cpuDescriptorHandle)
	{
	}
};

class SD3D12SamplerView : public IRDISamplerView, public SD3D12ViewBase
{
public:
	SD3D12SamplerView(D3D12_CPU_DESCRIPTOR_HANDLE _cpuDescriptorHandle)
		:SD3D12ViewBase(_cpuDescriptorHandle)
	{
	}
};

class SD3D12Buffer : public IRDIBuffer
{
public:
	void Init(ID3D12Resource* _nativePtr, const SRDIBufferResourceDesc* _desc, SD3D12Device* _device) noexcept;
	ID3D12Resource* GetNativePtr() noexcept { return mResourceNativePtr; }

public:
	void GetDesc(SRDIBufferResourceDesc* _desc)const noexcept override { *_desc = mDesc; }
	SBufferView Map() noexcept;
	void Unmap() noexcept;
	void Release() noexcept override;

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress() noexcept { return mGpuVirtualAddress; }

	IRDIIndexBufferView* GetIBV() noexcept override;
	IRDIVertexBufferView* GetVBV() noexcept override;
	IRDIShaderResourceView* GetSRV() noexcept override;
	IRDIUnorderedAccessView* GetUAV() noexcept override;

private:
	SD3D12Device* mDevice = nullptr;
	ID3D12Resource* mResourceNativePtr = nullptr;
	SRDIBufferResourceDesc mDesc;

	D3D12_GPU_VIRTUAL_ADDRESS mGpuVirtualAddress = 0;
	SD3D12IndexBufferView mIBV;
	SD3D12VertexBufferView mVBV;
	SD3D12ShaderResourceView* mSRV = nullptr;
	SD3D12UnorderedAccessView* mUAV = nullptr;
};

class SD3D12Texture1D : public IRDITexture1D
{
public:
	void Init(ID3D12Resource* _nativePtr, const SRDITexture1DResourceDesc* _desc, SD3D12Device* _device) noexcept;
	ID3D12Resource* GetNativePtr() noexcept { return mResourceNativePtr; }

public:
	void GetDesc(SRDITexture1DResourceDesc* _desc)const noexcept { *_desc = mDesc; }
	void Release() noexcept override;

	IRDIRenderTargetView* GetRTV(uint32_t _mipSlice) noexcept;
	IRDIDepthStencilView* GetDSV(uint32_t _mipSlice) noexcept;
	IRDIShaderResourceView* GetSRV() noexcept;
	IRDIUnorderedAccessView* GetUAV(uint32_t _mipSlice) noexcept;

private:
	SD3D12Device* mDevice = nullptr;
	ID3D12Resource* mResourceNativePtr = nullptr;
	SRDITexture1DResourceDesc mDesc;

	SD3D12RenderTargetView* mRTVs[D3D12_MIPMAPCOUNT] = {};
	SD3D12DepthStencilView* mDSVs[D3D12_MIPMAPCOUNT] = {};
	SD3D12ShaderResourceView* mSRV = nullptr;
	SD3D12UnorderedAccessView* mUAVs[D3D12_MIPMAPCOUNT] = {};
};

class SD3D12Texture1DArray : public IRDITexture1DArray
{
public:
	void Init(ID3D12Resource* _nativePtr, const SRDITexture1DArrayResourceDesc* _desc, SD3D12Device* _device) noexcept;
	ID3D12Resource* GetNativePtr() noexcept { return mResourceNativePtr; }

public:
	void GetDesc(SRDITexture1DArrayResourceDesc* _desc)const noexcept override { *_desc = mDesc; }
	void Release() noexcept override;

	IRDIShaderResourceView* GetSRV() noexcept;
	IRDIUnorderedAccessView* GetUAV(uint32_t _mipSlice) noexcept;

private:
	SD3D12Device* mDevice = nullptr;
	ID3D12Resource* mResourceNativePtr = nullptr;
	SRDITexture1DArrayResourceDesc mDesc;

	SD3D12ShaderResourceView* mSRV = nullptr;
	SD3D12UnorderedAccessView* mUAVs[D3D12_MIPMAPCOUNT] = {};
};

class SD3D12Texture2D : public IRDITexture2D
{
public:
	void Init(ID3D12Resource* _nativePtr, const SRDITexture2DResourceDesc* _desc, SD3D12Device* _device) noexcept;
	ID3D12Resource* GetNativePtr() noexcept { return mResourceNativePtr; }

public:
	void GetDesc(SRDITexture2DResourceDesc* _desc)const noexcept { *_desc = mDesc; }
	void Release() noexcept override;

	IRDIRenderTargetView* GetRTV(uint32_t _mipSlice) noexcept;
	IRDIDepthStencilView* GetDSV(uint32_t _mipSlice) noexcept;
	IRDIShaderResourceView* GetSRV() noexcept;
	IRDIUnorderedAccessView* GetUAV(uint32_t _mipSlice) noexcept;

private:
	SD3D12Device* mDevice = nullptr;
	ID3D12Resource* mResourceNativePtr = nullptr;
	SRDITexture2DResourceDesc mDesc;

	SD3D12RenderTargetView* mRTVs[D3D12_MIPMAPCOUNT] = {};
	SD3D12DepthStencilView* mDSVs[D3D12_MIPMAPCOUNT] = {};
	SD3D12ShaderResourceView* mSRV = nullptr;
	SD3D12UnorderedAccessView* mUAVs[D3D12_MIPMAPCOUNT] = {};
};

class SD3D12Texture2DArray : public IRDITexture2DArray
{
public:
	void Init(ID3D12Resource* _nativePtr, const SRDITexture2DArrayResourceDesc* _desc, SD3D12Device* _device) noexcept;
	ID3D12Resource* GetNativePtr() noexcept { return mResourceNativePtr; }

public:
	void GetDesc(SRDITexture2DArrayResourceDesc* _desc)const noexcept { *_desc = mDesc; }
	void Release() noexcept override;

	IRDIShaderResourceView* GetSRV() noexcept;
	IRDIUnorderedAccessView* GetUAV(uint32_t _mipSlice) noexcept;

private:
	SD3D12Device* mDevice = nullptr;
	ID3D12Resource* mResourceNativePtr = nullptr;
	SRDITexture2DArrayResourceDesc mDesc;

	SD3D12ShaderResourceView* mSRV = nullptr;
	SD3D12UnorderedAccessView* mUAVs[D3D12_MIPMAPCOUNT] = {};
};

class SD3D12Texture3D : public IRDITexture3D
{
public:
	void Init(ID3D12Resource* _nativePtr, const SRDITexture3DResourceDesc* _desc, SD3D12Device* _device) noexcept;
	ID3D12Resource* GetNativePtr() noexcept { return mResourceNativePtr; }

public:
	void GetDesc(SRDITexture3DResourceDesc* _desc)const noexcept { *_desc = mDesc; }
	void Release() noexcept override;

	IRDIRenderTargetView* GetRTV(uint32_t _mipSlice) noexcept;
	IRDIShaderResourceView* GetSRV() noexcept;
	IRDIUnorderedAccessView* GetUAV(uint32_t _mipSlice) noexcept;

private:
	SD3D12Device* mDevice = nullptr;
	ID3D12Resource* mResourceNativePtr = nullptr;
	SRDITexture3DResourceDesc mDesc;

	SD3D12RenderTargetView* mRTVs[D3D12_MIPMAPCOUNT] = {};
	SD3D12ShaderResourceView* mSRV = nullptr;
	SD3D12UnorderedAccessView* mUAVs[D3D12_MIPMAPCOUNT] = {};
};

class SD3D12TextureCube : public IRDITextureCube
{
public:
	void Init(ID3D12Resource* _nativePtr, const SRDITextureCubeResourceDesc* _desc, SD3D12Device* _device) noexcept;
	ID3D12Resource* GetNativePtr() noexcept { return mResourceNativePtr; }

public:
	void GetDesc(SRDITextureCubeResourceDesc* _desc) const noexcept { *_desc = mDesc; }
	void Release() noexcept override;

	IRDIRenderTargetView* GetRTV(ERDITextureCubeFace _face, uint32_t _mipSlice) noexcept;
	IRDIDepthStencilView* GetDSV(ERDITextureCubeFace _face, uint32_t _mipSlice) noexcept;
	IRDIShaderResourceView* GetSRV() noexcept;
	IRDIUnorderedAccessView* GetUAV(uint32_t _mipSlice) noexcept;

private:
	SD3D12Device* mDevice = nullptr;
	ID3D12Resource* mResourceNativePtr = nullptr;
	SRDITextureCubeResourceDesc mDesc;

	SD3D12RenderTargetView* mRTVs[D3D12_MIPMAPCOUNT * EnumToInt(ERDITextureCubeFace::FaceCount)] = {};
	SD3D12DepthStencilView* mDSVs[D3D12_MIPMAPCOUNT * EnumToInt(ERDITextureCubeFace::FaceCount)] = {};
	SD3D12ShaderResourceView* mSRV = nullptr;
	SD3D12UnorderedAccessView* mUAVs[D3D12_MIPMAPCOUNT * EnumToInt(ERDITextureCubeFace::FaceCount)] = {};
};

class SD3D12TextureCubeArray : public IRDITextureCubeArray
{
public:
	void Init(ID3D12Resource* _nativePtr, const SRDITextureCubeArrayResourceDesc* _desc, SD3D12Device* _device) noexcept;
	ID3D12Resource* GetNativePtr() noexcept { return mResourceNativePtr; }

public:
	void GetDesc(SRDITextureCubeArrayResourceDesc* _desc) const noexcept { *_desc = mDesc; }
	void Release() noexcept override;

	IRDIShaderResourceView* GetSRV() noexcept;
	IRDIUnorderedAccessView* GetUAV(uint32_t _mipSlice) noexcept;

private:
	SD3D12Device* mDevice = nullptr;
	ID3D12Resource* mResourceNativePtr = nullptr;
	SRDITextureCubeArrayResourceDesc mDesc;

	SD3D12ShaderResourceView* mSRV = nullptr;
	SD3D12UnorderedAccessView* mUAVs[D3D12_MIPMAPCOUNT * EnumToInt(ERDITextureCubeFace::FaceCount)] = {};
};

class SD3D12Sampler : public IRDISampler
{
public:
	void Init(const SRDISamplerResourceDesc* _desc, SD3D12Device* _device) noexcept;

public:
	void GetDesc(SRDISamplerResourceDesc* _desc) const noexcept { *_desc = mDesc; }
	void Release() noexcept override;

	IRDISamplerView* GetSamplerView() noexcept;

private:
	SD3D12Device* mDevice = nullptr;
	SRDISamplerResourceDesc mDesc = {};

	SD3D12SamplerView* mSamplerView;
};

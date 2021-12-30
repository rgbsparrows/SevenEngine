#pragma once

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

enum class ERDITextureCubeFace;

__interface IRDIIndexBufferView
{
};

__interface IRDIVertexBufferView
{
};

__interface IRDIRenderTargetView
{
};

__interface IRDIDepthStencilView
{
};

__interface IRDIShaderResourceView
{
};

__interface IRDIUnorderedAccessView
{
};

__interface IRDISamplerView
{
};

__interface IRDIResource
{
	void Release() noexcept;
};

__interface IRDIBuffer : IRDIResource
{
	void GetDesc(SRDIBufferResourceDesc* _desc)const noexcept;
	void Map(void** _dataPtr) noexcept;
	void Unmap() noexcept;

	IRDIIndexBufferView* GetIBV() noexcept;
	IRDIVertexBufferView* GetVBV() noexcept;
	IRDIShaderResourceView* GetSRV() noexcept;
	IRDIUnorderedAccessView* GetUAV() noexcept;
};

__interface IRDITexture : IRDIResource
{
};

__interface IRDITexture1D : IRDITexture
{
	void GetDesc(SRDITexture1DResourceDesc* _desc)const noexcept;

	IRDIRenderTargetView* GetRTV(uint32_t _mipSlice) noexcept;
	IRDIDepthStencilView* GetDSV(uint32_t _mipSlice) noexcept;
	IRDIShaderResourceView* GetSRV() noexcept;
	IRDIUnorderedAccessView* GetUAV(uint32_t _mipSlice) noexcept;
};

__interface IRDITexture1DArray : IRDITexture
{
	void GetDesc(SRDITexture1DArrayResourceDesc* _desc)const noexcept;

	IRDIShaderResourceView* GetSRV() noexcept;
	IRDIUnorderedAccessView* GetUAV(uint32_t _mipSlice) noexcept;
};

__interface IRDITexture2D : IRDITexture
{
	void GetDesc(SRDITexture2DResourceDesc* _desc)const noexcept;

	IRDIRenderTargetView* GetRTV(uint32_t _mipSlice) noexcept;
	IRDIDepthStencilView* GetDSV(uint32_t _mipSlice) noexcept;
	IRDIShaderResourceView* GetSRV() noexcept;
	IRDIUnorderedAccessView* GetUAV(uint32_t _mipSlice) noexcept;
};

__interface IRDITexture2DArray : IRDITexture
{
	void GetDesc(SRDITexture2DArrayResourceDesc* _desc)const noexcept;

	IRDIShaderResourceView* GetSRV() noexcept;
	IRDIUnorderedAccessView* GetUAV(uint32_t _mipSlice) noexcept;
};

__interface IRDITexture3D : IRDITexture
{
	void GetDesc(SRDITexture3DResourceDesc* _desc)const noexcept;

	IRDIRenderTargetView* GetRTV(uint32_t _mipSlice) noexcept;
	IRDIShaderResourceView* GetSRV() noexcept;
	IRDIUnorderedAccessView* GetUAV(uint32_t _mipSlice) noexcept;
};

__interface IRDITextureCube : IRDITexture
{
	void GetDesc(SRDITextureCubeResourceDesc* _desc) const noexcept;

	IRDIRenderTargetView* GetRTV(ERDITextureCubeFace _face, uint32_t _mipSlice) noexcept;
	IRDIDepthStencilView* GetDSV(ERDITextureCubeFace _face, uint32_t _mipSlice) noexcept;
	IRDIShaderResourceView* GetSRV() noexcept;
	IRDIUnorderedAccessView* GetUAV(uint32_t _mipSlice) noexcept;
};

__interface IRDITextureCubeArray : IRDITexture
{
	void GetDesc(SRDITextureCubeArrayResourceDesc* _desc) const noexcept;

	IRDIShaderResourceView* GetSRV() noexcept;
	IRDIUnorderedAccessView* GetUAV(uint32_t _mipSlice) noexcept;
};

__interface IRDISampler : IRDIResource
{
	void GetDesc(SRDISamplerResourceDesc* _desc) const noexcept;

	IRDISamplerView* GetSamplerView() noexcept;
};

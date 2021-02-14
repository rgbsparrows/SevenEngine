#include "D3D12DescriptorHeap.h"
#include "D3D/D3D12/D3D12Device.h"
#include "D3D/Warper/D3DEnumConvertor.h"
#include "D3D/D3D12/Warper/D3D12ImplWarper.h"
#include "D3D/D3D12/Warper/D3D12EnumConvertor.h"
#include "D3D/D3D12/Warper/D3D12ImplWarperHelper.h"

void SD3D12DescriptorHeapRange::Init(uint64_t _srvUavDescriptorHeapIndex, uint16_t _srvDescriptorCount, uint16_t _uavDescriptorCount, SD3D12ShaderVisibleDescriptorHeap* _shaderVisibleDescriptorHeap) noexcept
{
	mSrvUavDescriptorHeapIndex = _srvUavDescriptorHeapIndex;
	mSrvDescriptorCount = _srvDescriptorCount;
	mUavDescriptorCount = _uavDescriptorCount;

	mShaderVisibleDescriptorHeap = _shaderVisibleDescriptorHeap;

	{
		mCPUDescriptorHandle =
			mShaderVisibleDescriptorHeap->GetSrvUavDescriptorHeapStartCPUDescriptorHandle() +
			_srvUavDescriptorHeapIndex * mShaderVisibleDescriptorHeap->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		mGPUDescriptorHandle = 
			mShaderVisibleDescriptorHeap->GetSrvUavDescriptorHeapStartGPUDescriptorHandle() +
			_srvUavDescriptorHeapIndex * mShaderVisibleDescriptorHeap->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}
}

void SD3D12DescriptorHeapRange::SetSRV(IRDIShaderResourceView* _srv, uint64_t _offset) noexcept
{
	CHECK(_offset < mSrvDescriptorCount);
	mShaderVisibleDescriptorHeap->SetSRV(_srv, _offset + GetSrvDescriptorHeapIndex());
}

void SD3D12DescriptorHeapRange::SetUAV(IRDIUnorderedAccessView* _uav, uint64_t _offset) noexcept
{
	CHECK(_offset < mUavDescriptorCount);
	mShaderVisibleDescriptorHeap->SetUAV(_uav, _offset + GetUavDescriptorHeapIndex());
}

void SD3D12DescriptorHeapRange::Release() noexcept
{
	mShaderVisibleDescriptorHeap->ReleaseDescriptorHeapRange(this);
}


void SD3D12SamplerHeapRange::Init(uint64_t _samplerViewDescriptorHeapIndex, uint16_t _samplerViewDescriptorCount, SD3D12ShaderVisibleDescriptorHeap* _shaderVisibleDescriptorHeap) noexcept
{
	mSamplerViewDescriptorHeapIndex = _samplerViewDescriptorHeapIndex;
	mSamplerViewDescriptorCount = _samplerViewDescriptorCount;

	mShaderVisibleDescriptorHeap = _shaderVisibleDescriptorHeap;

	{
		mCPUDescriptorHandle =
			mShaderVisibleDescriptorHeap->GetSrvUavDescriptorHeapStartCPUDescriptorHandle() +
			_samplerViewDescriptorHeapIndex * mShaderVisibleDescriptorHeap->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

		mGPUDescriptorHandle =
			mShaderVisibleDescriptorHeap->GetSrvUavDescriptorHeapStartGPUDescriptorHandle() +
			_samplerViewDescriptorHeapIndex * mShaderVisibleDescriptorHeap->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	}
}

void SD3D12SamplerHeapRange::SetSampler(IRDISamplerView* _sampler, uint64_t _offset) noexcept
{
	CHECK(_offset < mSamplerViewDescriptorCount);
	mShaderVisibleDescriptorHeap->SetSampler(_sampler, _offset + mSamplerViewDescriptorHeapIndex);
}

void SD3D12SamplerHeapRange::Release() noexcept
{
	mShaderVisibleDescriptorHeap->ReleaseSamplerHeapRange(this);
}


void SD3D12ShaderVisibleDescriptorHeap::Init(void* _srvUavDescriptorHeapNativePtr, uint64_t _srvUavDescriptorCount, void* _samplerViewDescriptorHeapNativePtr, uint64_t _samplerViewDescriptorCount, SD3D12Device* _device) noexcept
{
	mSrvUavDescriptorHeapNativePtr = _srvUavDescriptorHeapNativePtr;
	mSamplerViewDescriptorHeapNativePtr = _samplerViewDescriptorHeapNativePtr;

	mSrvUavDescriptorAlloctorHelper.ResetSlotCount(_srvUavDescriptorCount);
	mSamplerViewAlloctorHelper.ResetSlotCount(_samplerViewDescriptorCount);

	D3D12APIWarp_Impl::D3D12GetCPUDescriptorHandleForHeapStart_D3D12Impl(mSrvUavDescriptorHeapNativePtr, &mSrvUavDescriptorHeapStartCPUDescriptorHandle);
	D3D12APIWarp_Impl::D3D12GetCPUDescriptorHandleForHeapStart_D3D12Impl(mSamplerViewDescriptorHeapNativePtr, &mSamplerViewDescriptorHeapStartCPUDescriptorHandle);

	D3D12APIWarp_Impl::D3D12GetGPUDescriptorHandleForHeapStart_D3D12Impl(mSrvUavDescriptorHeapNativePtr, &mSrvUavDescriptorHeapStartGPUDescriptorHandle);
	D3D12APIWarp_Impl::D3D12GetGPUDescriptorHandleForHeapStart_D3D12Impl(mSamplerViewDescriptorHeapNativePtr, &mSamplerViewDescriptorHeapStartGPUDescriptorHandle);

	mDevice = _device;
}

SD3D12DescriptorHeapRange* SD3D12ShaderVisibleDescriptorHeap::AllocateDescriptorHeapRange(uint16_t _srvDescriptorCount, uint16_t _uavDescriptorCount) noexcept
{
	uint64_t srvUavStart = mSrvUavDescriptorAlloctorHelper.AllocateSlotArray(0ull + _srvDescriptorCount + _uavDescriptorCount);

	CHECK(srvUavStart != std::numeric_limits<uint64_t>::max() && L"描述符堆中没有充足的空间以为新资源分配描述符");

	SD3D12DescriptorHeapRange descriptorHeapRange;
	descriptorHeapRange.Init(srvUavStart, _srvDescriptorCount, _uavDescriptorCount, this);

	return mDescriptorHeapRangePool.AllocateElement(descriptorHeapRange);
}

SD3D12SamplerHeapRange* SD3D12ShaderVisibleDescriptorHeap::AllocateSamplerHeapRange(uint16_t _samplerViewDescriptorCount) noexcept
{
	uint64_t samplerViewStart = mSamplerViewAlloctorHelper.AllocateSlotArray(_samplerViewDescriptorCount);

	CHECK(samplerViewStart != std::numeric_limits<uint64_t>::max() && L"Sampler描述符堆中没有充足的空间以为新资源分配描述符");

	SD3D12SamplerHeapRange descriptorHeapRange;
	descriptorHeapRange.Init(samplerViewStart, _samplerViewDescriptorCount, this);

	return mSamplerHeapRangePool.AllocateElement(descriptorHeapRange);
}

void SD3D12ShaderVisibleDescriptorHeap::ReleaseDescriptorHeapRange(IRDIDescriptorHeapRange* _descriptorHeapRange) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_descriptorHeapRange);

	mSrvUavDescriptorAlloctorHelper.DeallocateSlotArray(descriptorHeapRange->GetSrvUavDescriptorHeapIndex(), 0ull + descriptorHeapRange->GetSrvDescriptorCount() + descriptorHeapRange->GetUavDescriptorCount());
	mDescriptorHeapRangePool.DeallocateElement(descriptorHeapRange);
}

void SD3D12ShaderVisibleDescriptorHeap::ReleaseSamplerHeapRange(IRDISamplerHeapRange* _samplerHeapRange) noexcept
{
	SD3D12SamplerHeapRange* samplerHeapRange = static_cast<SD3D12SamplerHeapRange*>(_samplerHeapRange);

	mSamplerViewAlloctorHelper.DeallocateSlotArray(samplerHeapRange->GetSamplerViewDescriptorHeapIndex(), samplerHeapRange->GetSamplerViewDescriptorCount());
	mSamplerHeapRangePool.DeallocateElement(samplerHeapRange);
}

void SD3D12ShaderVisibleDescriptorHeap::SetSRV(IRDIShaderResourceView* _srv, uint64_t _index) noexcept
{
	size_t destDescriptorHeapStartCPUDescriptorHandle = mSrvUavDescriptorHeapStartCPUDescriptorHandle + GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * _index;
	size_t srcDescriptorHeapStartCPUDescriptorHandle = static_cast<SD3D12ShaderResourceView*>(_srv)->mCpuDescriptorHandle;
	D3D12APIWarp_Impl::D3D12CopyDescriptorsSimple_D3D12Impl(mDevice->GetNativePtr(), 1, destDescriptorHeapStartCPUDescriptorHandle, srcDescriptorHeapStartCPUDescriptorHandle, static_cast<int32_t>(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
}

void SD3D12ShaderVisibleDescriptorHeap::SetUAV(IRDIUnorderedAccessView* _uav, uint64_t _index) noexcept
{
	size_t destDescriptorHeapStartCPUDescriptorHandle = mSrvUavDescriptorHeapStartCPUDescriptorHandle + GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * _index;
	size_t srcDescriptorHeapStartCPUDescriptorHandle = static_cast<SD3D12UnorderedAccessView*>(_uav)->mCpuDescriptorHandle;
	D3D12APIWarp_Impl::D3D12CopyDescriptorsSimple_D3D12Impl(mDevice->GetNativePtr(), 1, destDescriptorHeapStartCPUDescriptorHandle, srcDescriptorHeapStartCPUDescriptorHandle, static_cast<int32_t>(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
}

void SD3D12ShaderVisibleDescriptorHeap::SetSampler(IRDISamplerView* _sampler, uint64_t _index) noexcept
{
	size_t destDescriptorHeapStartCPUDescriptorHandle = mSrvUavDescriptorHeapStartCPUDescriptorHandle + GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER) * _index;
	size_t srcDescriptorHeapStartCPUDescriptorHandle = static_cast<SD3D12SamplerView*>(_sampler)->mCpuDescriptorHandle;
	D3D12APIWarp_Impl::D3D12CopyDescriptorsSimple_D3D12Impl(mDevice->GetNativePtr(), 1, destDescriptorHeapStartCPUDescriptorHandle, srcDescriptorHeapStartCPUDescriptorHandle, static_cast<int32_t>(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER));
}

uint32_t SD3D12ShaderVisibleDescriptorHeap::GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE _descriptorHeapType) noexcept
{
	return mDevice->GetDescriptorHandleIncrement(_descriptorHeapType);
}

void SD3D12DescriptorHeap::Init(void* _rtvDescriptorHeapNativePtr, uint64_t _rtvDescriptorCount, void* _dsvDescriptorHeapNativePtr, uint64_t _dsvDescriptorCount, void* _srvDescriptorHeapNativePtr, uint64_t _srvDescriptorCount, void* _uavDescriptorHeapNativePtr, uint64_t _uavDescriptorCount, void* _samplerViewDescriptorHeapNativePtr, uint64_t _samplerViewDescriptorCount, SD3D12Device* _device) noexcept
{
	mRTVDescriptorHeapNativePtr = _rtvDescriptorHeapNativePtr;
	mDSVDescriptorHeapNativePtr = _dsvDescriptorHeapNativePtr;
	mSRVDescriptorHeapNativePtr = _srvDescriptorHeapNativePtr;
	mUAVDescriptorHeapNativePtr = _uavDescriptorHeapNativePtr;
	mSamplerViewDescriptorHeapNativePtr = _samplerViewDescriptorHeapNativePtr;

	mRTVAllocatorHelper.ResetSlotCount(_rtvDescriptorCount);
	mDSVAllocatorHelper.ResetSlotCount(_dsvDescriptorCount);
	mSRVAllocatorHelper.ResetSlotCount(_srvDescriptorCount);
	mUAVAllocatorHelper.ResetSlotCount(_uavDescriptorCount);
	mSamplerViewAllocatorHelper.ResetSlotCount(_samplerViewDescriptorCount);

	mRenderTargetViewPool.ResizePool(_rtvDescriptorCount);
	mDepthStencilViewPool.ResizePool(_dsvDescriptorCount);
	mShaderResourceViewPool.ResizePool(_srvDescriptorCount);
	mUnorderedAccessViewPool.ResizePool(_uavDescriptorCount);
	mSamplerViewPool.ResizePool(_samplerViewDescriptorCount);

	D3D12APIWarp_Impl::D3D12GetCPUDescriptorHandleForHeapStart_D3D12Impl(mRTVDescriptorHeapNativePtr, &mRTVDescriptorHeapStartCPUDescriptorHandle);
	D3D12APIWarp_Impl::D3D12GetCPUDescriptorHandleForHeapStart_D3D12Impl(mDSVDescriptorHeapNativePtr, &mDSVDescriptorHeapStartCPUDescriptorHandle);
	D3D12APIWarp_Impl::D3D12GetCPUDescriptorHandleForHeapStart_D3D12Impl(mSRVDescriptorHeapNativePtr, &mSRVDescriptorHeapStartCPUDescriptorHandle);
	D3D12APIWarp_Impl::D3D12GetCPUDescriptorHandleForHeapStart_D3D12Impl(mUAVDescriptorHeapNativePtr, &mUAVDescriptorHeapStartCPUDescriptorHandle);
	D3D12APIWarp_Impl::D3D12GetCPUDescriptorHandleForHeapStart_D3D12Impl(mSamplerViewDescriptorHeapNativePtr, &mSamplerViewDescriptorHeapStartCPUDescriptorHandle);

	D3D12APIWarp_Impl::D3D12GetGPUDescriptorHandleForHeapStart_D3D12Impl(mRTVDescriptorHeapNativePtr, &mRTVDescriptorHeapStartGPUDescriptorHandle);
	D3D12APIWarp_Impl::D3D12GetGPUDescriptorHandleForHeapStart_D3D12Impl(mDSVDescriptorHeapNativePtr, &mDSVDescriptorHeapStartGPUDescriptorHandle);
	D3D12APIWarp_Impl::D3D12GetGPUDescriptorHandleForHeapStart_D3D12Impl(mSRVDescriptorHeapNativePtr, &mSRVDescriptorHeapStartGPUDescriptorHandle);
	D3D12APIWarp_Impl::D3D12GetGPUDescriptorHandleForHeapStart_D3D12Impl(mUAVDescriptorHeapNativePtr, &mUAVDescriptorHeapStartGPUDescriptorHandle);
	D3D12APIWarp_Impl::D3D12GetGPUDescriptorHandleForHeapStart_D3D12Impl(mSamplerViewDescriptorHeapNativePtr, &mSamplerViewDescriptorHeapStartGPUDescriptorHandle);

	mDevice = _device;
}

SD3D12RenderTargetView* SD3D12DescriptorHeap::CreateRTV(IRDITexture1D* _resource, uint32_t _mipSlice) noexcept
{
	SD3D12Texture1D* texture1d = static_cast<SD3D12Texture1D*>(_resource);
	SRDITexture1DResourceDesc desc;
	texture1d->GetDesc(&desc);

	ERDIPixelFormat format = desc.mRtvPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = ConvertPixelFormatToD3D(format);
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE1D;
	rtvDesc.Texture1D.MipSlice = _mipSlice;

	return CreateRTV(texture1d->GetNativePtr(), &rtvDesc);
}

SD3D12RenderTargetView* SD3D12DescriptorHeap::CreateRTV(IRDITexture2D* _resource, uint32_t _mipSlice) noexcept
{
	SD3D12Texture2D* texture2d = static_cast<SD3D12Texture2D*>(_resource);
	SRDITexture2DResourceDesc desc;
	texture2d->GetDesc(&desc);

	ERDIPixelFormat format = desc.mRtvPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = ConvertPixelFormatToD3D(format);
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = _mipSlice;
	rtvDesc.Texture2D.PlaneSlice = 0;

	return CreateRTV(texture2d->GetNativePtr(), &rtvDesc);
}

SD3D12RenderTargetView* SD3D12DescriptorHeap::CreateRTV(IRDITexture3D* _resource, uint32_t _mipSlice) noexcept
{
	SD3D12Texture3D* texture3d = static_cast<SD3D12Texture3D*>(_resource);
	SRDITexture3DResourceDesc desc;
	texture3d->GetDesc(&desc);

	ERDIPixelFormat format = desc.mRtvPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = ConvertPixelFormatToD3D(format);
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE3D;
	rtvDesc.Texture3D.MipSlice = _mipSlice;
	rtvDesc.Texture3D.FirstWSlice = 0;
	rtvDesc.Texture3D.WSize = desc.mSizeZ;

	return CreateRTV(texture3d->GetNativePtr(), &rtvDesc);
}

SD3D12RenderTargetView* SD3D12DescriptorHeap::CreateRTV(IRDITextureCube* _resource, ERDITextureCubeFace _cubeFace, uint32_t _mipSlice) noexcept
{
	SD3D12TextureCube* textureCube = static_cast<SD3D12TextureCube*>(_resource);
	SRDITextureCubeResourceDesc desc;
	textureCube->GetDesc(&desc);

	ERDIPixelFormat format = desc.mRtvPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = ConvertPixelFormatToD3D(format);
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
	rtvDesc.Texture2DArray.MipSlice = _mipSlice;
	rtvDesc.Texture2DArray.FirstArraySlice = EnumToInt(_cubeFace);
	rtvDesc.Texture2DArray.ArraySize = 1;
	rtvDesc.Texture2DArray.PlaneSlice = 0;

	return CreateRTV(textureCube->GetNativePtr(), &rtvDesc);
}

SD3D12DepthStencilView* SD3D12DescriptorHeap::CreateDSV(IRDITexture1D* _resource, uint32_t _mipSlice) noexcept
{
	SD3D12Texture1D* texture1d = static_cast<SD3D12Texture1D*>(_resource);
	SRDITexture1DResourceDesc desc;
	texture1d->GetDesc(&desc);

	ERDIPixelFormat format = desc.mDsvPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = ConvertPixelFormatToD3D(format);
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE1D;
	dsvDesc.Flags = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
	dsvDesc.Texture1D.MipSlice = _mipSlice;

	return CreateDSV(texture1d->GetNativePtr(), &dsvDesc);
}

SD3D12DepthStencilView* SD3D12DescriptorHeap::CreateDSV(IRDITexture2D* _resource, uint32_t _mipSlice) noexcept
{
	SD3D12Texture2D* texture2d = static_cast<SD3D12Texture2D*>(_resource);
	SRDITexture2DResourceDesc desc;
	texture2d->GetDesc(&desc);

	ERDIPixelFormat format = desc.mDsvPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = ConvertPixelFormatToD3D(format);
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
	dsvDesc.Texture2D.MipSlice = _mipSlice;

	return CreateDSV(texture2d->GetNativePtr(), &dsvDesc);
}

SD3D12DepthStencilView* SD3D12DescriptorHeap::CreateDSV(IRDITextureCube* _resource, ERDITextureCubeFace _cubeFace, uint32_t _mipSlice) noexcept
{
	SD3D12TextureCube* textureCube = static_cast<SD3D12TextureCube*>(_resource);
	SRDITextureCubeResourceDesc desc;
	textureCube->GetDesc(&desc);

	ERDIPixelFormat format = desc.mDsvPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = ConvertPixelFormatToD3D(format);
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
	dsvDesc.Flags = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
	dsvDesc.Texture2DArray.MipSlice = _mipSlice;
	dsvDesc.Texture2DArray.FirstArraySlice = EnumToInt(_cubeFace);
	dsvDesc.Texture2DArray.ArraySize = 1;

	return CreateDSV(textureCube->GetNativePtr(), &dsvDesc);
}

SD3D12ShaderResourceView* SD3D12DescriptorHeap::CreateSRV(IRDIBuffer* _resource) noexcept
{
	SD3D12Buffer* buffer = static_cast<SD3D12Buffer*>(_resource);
	SRDIBufferResourceDesc desc;
	buffer->GetDesc(&desc);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	if (desc.mElementStride != 0)
	{
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = static_cast<uint32_t>(desc.mBufferSize / desc.mElementStride);
		srvDesc.Buffer.StructureByteStride = desc.mElementStride;
		srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_NONE;
	}
	else
	{
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = 0;
		srvDesc.Buffer.StructureByteStride = 0;
		srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_RAW;
	}

	return CreateSRV(buffer->GetNativePtr(), &srvDesc);
}

SD3D12ShaderResourceView* SD3D12DescriptorHeap::CreateSRV(IRDITexture1D* _resource) noexcept
{
	SD3D12Texture1D* texture1d = static_cast<SD3D12Texture1D*>(_resource);
	SRDITexture1DResourceDesc desc;
	texture1d->GetDesc(&desc);

	ERDIPixelFormat format = desc.mSrvPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = ConvertPixelFormatToD3D(format);
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE1D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture1D.MostDetailedMip = 0;
	srvDesc.Texture1D.MipLevels = desc.mMipCount;
	srvDesc.Texture1D.ResourceMinLODClamp = 0.f;

	return CreateSRV(texture1d->GetNativePtr(), &srvDesc);
}

SD3D12ShaderResourceView* SD3D12DescriptorHeap::CreateSRV(IRDITexture1DArray* _resource) noexcept
{
	SD3D12Texture1DArray* texture1dArray = static_cast<SD3D12Texture1DArray*>(_resource);
	SRDITexture1DArrayResourceDesc desc;
	texture1dArray->GetDesc(&desc);

	ERDIPixelFormat format = desc.mSrvPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = ConvertPixelFormatToD3D(format);
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture1DArray.MostDetailedMip = 0;
	srvDesc.Texture1DArray.MipLevels = desc.mMipCount;
	srvDesc.Texture1DArray.FirstArraySlice = 0;
	srvDesc.Texture1DArray.ArraySize = desc.mArraySize;
	srvDesc.Texture1DArray.ResourceMinLODClamp = 0.f;

	return CreateSRV(texture1dArray->GetNativePtr(), &srvDesc);
}

SD3D12ShaderResourceView* SD3D12DescriptorHeap::CreateSRV(IRDITexture2D* _resource) noexcept
{
	SD3D12Texture2D* texture2d = static_cast<SD3D12Texture2D*>(_resource);
	SRDITexture2DResourceDesc desc;
	texture2d->GetDesc(&desc);

	ERDIPixelFormat format = desc.mSrvPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = ConvertPixelFormatToD3D(format);
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = desc.mMipCount;
	srvDesc.Texture2D.PlaneSlice = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.f;

	return CreateSRV(texture2d->GetNativePtr(), &srvDesc);
}

SD3D12ShaderResourceView* SD3D12DescriptorHeap::CreateSRV(IRDITexture2DArray* _resource) noexcept
{
	SD3D12Texture2DArray* texture2dArray = static_cast<SD3D12Texture2DArray*>(_resource);
	SRDITexture2DArrayResourceDesc desc;
	texture2dArray->GetDesc(&desc);

	ERDIPixelFormat format = desc.mSrvPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = ConvertPixelFormatToD3D(format);
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.MipLevels = desc.mMipCount;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.ArraySize = desc.mArraySize;
	srvDesc.Texture2DArray.PlaneSlice = 0;
	srvDesc.Texture2DArray.ResourceMinLODClamp = 0.f;

	return CreateSRV(texture2dArray->GetNativePtr(), &srvDesc);
}

SD3D12ShaderResourceView* SD3D12DescriptorHeap::CreateSRV(IRDITexture3D* _resource) noexcept
{
	SD3D12Texture3D* texture3d = static_cast<SD3D12Texture3D*>(_resource);
	SRDITexture3DResourceDesc desc;
	texture3d->GetDesc(&desc);

	ERDIPixelFormat format = desc.mSrvPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = ConvertPixelFormatToD3D(format);
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE3D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture3D.MostDetailedMip = 0;
	srvDesc.Texture3D.MipLevels = desc.mMipCount;
	srvDesc.Texture3D.ResourceMinLODClamp = 0.f;

	return CreateSRV(texture3d->GetNativePtr(), &srvDesc);
}

SD3D12ShaderResourceView* SD3D12DescriptorHeap::CreateSRV(IRDITextureCube* _resource) noexcept
{
	SD3D12TextureCube* textureCube = static_cast<SD3D12TextureCube*>(_resource);
	SRDITextureCubeResourceDesc desc;
	textureCube->GetDesc(&desc);

	ERDIPixelFormat format = desc.mSrvPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = ConvertPixelFormatToD3D(format);
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = desc.mMipCount;
	srvDesc.TextureCube.ResourceMinLODClamp = 0.f;

	return CreateSRV(textureCube->GetNativePtr(), &srvDesc);
}

SD3D12ShaderResourceView* SD3D12DescriptorHeap::CreateSRV(IRDITextureCubeArray* _resource) noexcept
{
	SD3D12TextureCubeArray* textureCubeArray = static_cast<SD3D12TextureCubeArray*>(_resource);
	SRDITextureCubeArrayResourceDesc desc;
	textureCubeArray->GetDesc(&desc);

	ERDIPixelFormat format = desc.mSrvPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = ConvertPixelFormatToD3D(format);
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.TextureCubeArray.MostDetailedMip = 0;
	srvDesc.TextureCubeArray.MipLevels = desc.mMipCount;
	srvDesc.TextureCubeArray.First2DArrayFace = 0;
	srvDesc.TextureCubeArray.NumCubes = desc.mArraySize;
	srvDesc.TextureCubeArray.ResourceMinLODClamp = 0.f;

	return CreateSRV(textureCubeArray->GetNativePtr(), &srvDesc);
}

SD3D12UnorderedAccessView* SD3D12DescriptorHeap::CreateUAV(IRDIBuffer* _resource) noexcept
{
	SD3D12Buffer* buffer = static_cast<SD3D12Buffer*>(_resource);
	SRDIBufferResourceDesc desc;
	buffer->GetDesc(&desc);

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = static_cast<uint32_t>(desc.mBufferSize / desc.mElementStride);
	uavDesc.Buffer.StructureByteStride = desc.mElementStride;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;

	return CreateUAV(buffer->GetNativePtr(), &uavDesc);
}

SD3D12UnorderedAccessView* SD3D12DescriptorHeap::CreateUAV(IRDITexture1D* _resource, uint32_t _mipSlice) noexcept
{
	SD3D12Texture1D* texture1d = static_cast<SD3D12Texture1D*>(_resource);
	SRDITexture1DResourceDesc desc;
	texture1d->GetDesc(&desc);

	ERDIPixelFormat format = desc.mUavPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = ConvertPixelFormatToD3D(format);
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE1D;
	uavDesc.Texture1D.MipSlice = _mipSlice;

	return CreateUAV(texture1d->GetNativePtr(), &uavDesc);
}

SD3D12UnorderedAccessView* SD3D12DescriptorHeap::CreateUAV(IRDITexture1DArray* _resource, uint32_t _mipSlice) noexcept
{
	SD3D12Texture1DArray* texture1dArray = static_cast<SD3D12Texture1DArray*>(_resource);
	SRDITexture1DArrayResourceDesc desc;
	texture1dArray->GetDesc(&desc);

	ERDIPixelFormat format = desc.mUavPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = ConvertPixelFormatToD3D(format);
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE1DARRAY;
	uavDesc.Texture1DArray.MipSlice = _mipSlice;
	uavDesc.Texture1DArray.FirstArraySlice = 0;
	uavDesc.Texture1DArray.ArraySize = desc.mArraySize;

	return CreateUAV(texture1dArray->GetNativePtr(), &uavDesc);
}

SD3D12UnorderedAccessView* SD3D12DescriptorHeap::CreateUAV(IRDITexture2D* _resource, uint32_t _mipSlice) noexcept
{
	SD3D12Texture2D* texture2d = static_cast<SD3D12Texture2D*>(_resource);
	SRDITexture2DResourceDesc desc;
	texture2d->GetDesc(&desc);

	ERDIPixelFormat format = desc.mUavPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = ConvertPixelFormatToD3D(format);
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = _mipSlice;
	uavDesc.Texture2D.PlaneSlice = 0;

	return CreateUAV(texture2d->GetNativePtr(), &uavDesc);
}

SD3D12UnorderedAccessView* SD3D12DescriptorHeap::CreateUAV(IRDITexture2DArray* _resource, uint32_t _mipSlice) noexcept
{
	SD3D12Texture2DArray* texture2dArray = static_cast<SD3D12Texture2DArray*>(_resource);
	SRDITexture2DArrayResourceDesc desc;
	texture2dArray->GetDesc(&desc);

	ERDIPixelFormat format = desc.mUavPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = ConvertPixelFormatToD3D(format);
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
	uavDesc.Texture2DArray.MipSlice = _mipSlice;
	uavDesc.Texture2DArray.FirstArraySlice = 0;
	uavDesc.Texture2DArray.ArraySize = desc.mArraySize;
	uavDesc.Texture2DArray.PlaneSlice = 0;

	return CreateUAV(texture2dArray->GetNativePtr(), &uavDesc);
}

SD3D12UnorderedAccessView* SD3D12DescriptorHeap::CreateUAV(IRDITexture3D* _resource, uint32_t _mipSlice) noexcept
{
	SD3D12Texture3D* texture3d = static_cast<SD3D12Texture3D*>(_resource);
	SRDITexture3DResourceDesc desc;
	texture3d->GetDesc(&desc);

	ERDIPixelFormat format = desc.mUavPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = ConvertPixelFormatToD3D(format);
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE3D;
	uavDesc.Texture3D.MipSlice = _mipSlice;
	uavDesc.Texture3D.FirstWSlice = 0;
	uavDesc.Texture3D.WSize = desc.mSizeZ;

	return CreateUAV(texture3d->GetNativePtr(), &uavDesc);
}

SD3D12UnorderedAccessView* SD3D12DescriptorHeap::CreateUAV(IRDITextureCube* _resource, uint32_t _mipSlice) noexcept
{
	SD3D12TextureCube* textureCube = static_cast<SD3D12TextureCube*>(_resource);
	SRDITextureCubeResourceDesc desc;
	textureCube->GetDesc(&desc);

	ERDIPixelFormat format = desc.mUavPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = ConvertPixelFormatToD3D(format);
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
	uavDesc.Texture2DArray.MipSlice = _mipSlice;
	uavDesc.Texture2DArray.FirstArraySlice = 0;
	uavDesc.Texture2DArray.ArraySize = 6;
	uavDesc.Texture2DArray.PlaneSlice = 0;

	return CreateUAV(textureCube->GetNativePtr(), &uavDesc);
}

SD3D12UnorderedAccessView* SD3D12DescriptorHeap::CreateUAV(IRDITextureCubeArray* _resource, uint32_t _mipSlice) noexcept
{
	SD3D12TextureCubeArray* textureCubeArray = static_cast<SD3D12TextureCubeArray*>(_resource);
	SRDITextureCubeArrayResourceDesc desc;
	textureCubeArray->GetDesc(&desc);

	ERDIPixelFormat format = desc.mUavPixelFormat;
	if (format == ERDIPixelFormat::UNKNOWN)
		format = desc.mPixelFormat;

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = ConvertPixelFormatToD3D(format);
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
	uavDesc.Texture2DArray.MipSlice = _mipSlice;
	uavDesc.Texture2DArray.FirstArraySlice = 0;
	uavDesc.Texture2DArray.ArraySize = desc.mArraySize * 6;
	uavDesc.Texture2DArray.PlaneSlice = 0;

	return CreateUAV(textureCubeArray->GetNativePtr(), &uavDesc);
}

SD3D12SamplerView* SD3D12DescriptorHeap::CreateSamplerView(IRDISampler* _sampler) noexcept
{
	SD3D12Sampler* sampler = static_cast<SD3D12Sampler*>(_sampler);
	SRDISamplerResourceDesc desc;
	sampler->GetDesc(&desc);

	D3D12_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = ConvertFilterToD3D12(desc.mFilter);
	samplerDesc.AddressU = ConvertAddressModeToD3D12(desc.mAddressU);
	samplerDesc.AddressV = ConvertAddressModeToD3D12(desc.mAddressV);
	samplerDesc.AddressW = ConvertAddressModeToD3D12(desc.mAddressW);
	samplerDesc.MipLODBias = desc.mMipLODBias;
	samplerDesc.MaxAnisotropy = desc.mMaxAnisotropy;
	samplerDesc.ComparisonFunc = ConvertComparisonFuncToD3D12(desc.mComparisonFunc);
	samplerDesc.BorderColor[0] = desc.mBorderColor.R;
	samplerDesc.BorderColor[1] = desc.mBorderColor.G;
	samplerDesc.BorderColor[2] = desc.mBorderColor.B;
	samplerDesc.BorderColor[3] = desc.mBorderColor.A;
	samplerDesc.MinLOD = desc.mMinLod;
	samplerDesc.MaxLOD = desc.mMaxLod;

	return CreateSamplerView(&samplerDesc);
}

void SD3D12DescriptorHeap::ReleaseRTV(SD3D12RenderTargetView* _rtv) noexcept
{
	uint64_t index = (_rtv->mCpuDescriptorHandle - mRTVDescriptorHeapStartCPUDescriptorHandle) / mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	mRTVAllocatorHelper.DeallocateSlot(index);
	mRenderTargetViewPool.DeallocateElement(_rtv);
}

void SD3D12DescriptorHeap::ReleaseDSV(SD3D12DepthStencilView* _dsv) noexcept
{
	uint64_t index = (_dsv->mCpuDescriptorHandle - mDSVDescriptorHeapStartCPUDescriptorHandle) / mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	mDSVAllocatorHelper.DeallocateSlot(index);
	mDepthStencilViewPool.DeallocateElement(_dsv);
}

void SD3D12DescriptorHeap::ReleaseSRV(SD3D12ShaderResourceView* _srv) noexcept
{
	uint64_t index = (_srv->mCpuDescriptorHandle - mSRVDescriptorHeapStartCPUDescriptorHandle) / mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	mSRVAllocatorHelper.DeallocateSlot(index);
	mShaderResourceViewPool.DeallocateElement(_srv);
}

void SD3D12DescriptorHeap::ReleaseUAV(SD3D12UnorderedAccessView* _uav) noexcept
{
	uint64_t index = (_uav->mCpuDescriptorHandle - mUAVDescriptorHeapStartCPUDescriptorHandle) / mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	mUAVAllocatorHelper.DeallocateSlot(index);
	mUnorderedAccessViewPool.DeallocateElement(_uav);
}

void SD3D12DescriptorHeap::ReleaseSamplerView(SD3D12SamplerView* _samplerView) noexcept
{
	uint64_t index = (_samplerView->mCpuDescriptorHandle - mSamplerViewDescriptorHeapStartCPUDescriptorHandle) / mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

	mSamplerViewAllocatorHelper.DeallocateSlot(index);
	mSamplerViewPool.DeallocateElement(_samplerView);
}

SD3D12RenderTargetView* SD3D12DescriptorHeap::CreateRTV(void* _resourceNativePtr, const D3D12_RENDER_TARGET_VIEW_DESC* _desc) noexcept
{
	uint64_t slotIndex = mRTVAllocatorHelper.AllocateSlot();

	CHECK(slotIndex != std::numeric_limits<uint64_t>::max());

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = mRTVDescriptorHeapStartCPUDescriptorHandle + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = mRTVDescriptorHeapStartGPUDescriptorHandle + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12APIWarp_Impl::D3D12CreateRenderTargetView_D3D12Impl(_resourceNativePtr, _desc, cpuHandle, mDevice->GetNativePtr());

	return mRenderTargetViewPool.AllocateElement(cpuHandle, gpuHandle);
}

SD3D12DepthStencilView* SD3D12DescriptorHeap::CreateDSV(void* _resourceNativePtr, const D3D12_DEPTH_STENCIL_VIEW_DESC* _desc) noexcept
{
	uint64_t slotIndex = mDSVAllocatorHelper.AllocateSlot();

	CHECK(slotIndex != std::numeric_limits<uint64_t>::max());

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = mDSVDescriptorHeapStartCPUDescriptorHandle + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = mDSVDescriptorHeapStartGPUDescriptorHandle + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	D3D12APIWarp_Impl::D3D12CreateDepthStencilView_D3D12Impl(_resourceNativePtr, _desc, cpuHandle, mDevice->GetNativePtr());

	return mDepthStencilViewPool.AllocateElement(cpuHandle, gpuHandle);
}

SD3D12ShaderResourceView* SD3D12DescriptorHeap::CreateSRV(void* _resourceNativePtr, const D3D12_SHADER_RESOURCE_VIEW_DESC* _desc) noexcept
{
	uint64_t slotIndex = mSRVAllocatorHelper.AllocateSlot();

	CHECK(slotIndex != std::numeric_limits<uint64_t>::max());

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = mSRVDescriptorHeapStartCPUDescriptorHandle + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = mSRVDescriptorHeapStartGPUDescriptorHandle + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12APIWarp_Impl::D3D12CreateShaderResourceView_D3D12Impl(_resourceNativePtr, _desc, cpuHandle, mDevice->GetNativePtr());

	return mShaderResourceViewPool.AllocateElement(cpuHandle, gpuHandle);
}

SD3D12UnorderedAccessView* SD3D12DescriptorHeap::CreateUAV(void* _resourceNativePtr, const D3D12_UNORDERED_ACCESS_VIEW_DESC* _desc) noexcept
{
	uint64_t slotIndex = mUAVAllocatorHelper.AllocateSlot();

	CHECK(slotIndex != std::numeric_limits<uint64_t>::max());

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = mUAVDescriptorHeapStartCPUDescriptorHandle + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = mUAVDescriptorHeapStartGPUDescriptorHandle + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12APIWarp_Impl::D3D12CreateUnorderedAccessView_D3D12Impl(_resourceNativePtr, nullptr, _desc, cpuHandle, mDevice->GetNativePtr());

	return mUnorderedAccessViewPool.AllocateElement(cpuHandle, gpuHandle);
}

SD3D12SamplerView* SD3D12DescriptorHeap::CreateSamplerView(const D3D12_SAMPLER_DESC* _desc) noexcept
{
	uint64_t slotIndex = mSamplerViewAllocatorHelper.AllocateSlot();

	CHECK(slotIndex != std::numeric_limits<uint64_t>::max());

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = mSamplerViewDescriptorHeapStartCPUDescriptorHandle + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = mSamplerViewDescriptorHeapStartGPUDescriptorHandle + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

	D3D12APIWarp_Impl::D3D12CreateSamplerView_D3D12Impl(_desc, cpuHandle, mDevice->GetNativePtr());

	return mSamplerViewPool.AllocateElement(cpuHandle, gpuHandle);
}

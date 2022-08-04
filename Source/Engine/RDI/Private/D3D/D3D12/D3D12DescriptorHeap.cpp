#include "D3D12DescriptorHeap.h"
#include "D3D/D3D12/D3D12Device.h"
#include "D3D/Helper/D3DEnumConvertor.h"
#include "D3D/D3D12/Helper/D3D12EnumConvertor.h"

void SD3D12DescriptorHeapRange::Init(uint64_t _srvUavDescriptorHeapIndex, uint16_t _srvDescriptorCount, uint16_t _uavDescriptorCount, SD3D12ShaderVisibleDescriptorHeap* _shaderVisibleDescriptorHeap) noexcept
{
	mSrvUavDescriptorHeapIndex = _srvUavDescriptorHeapIndex;
	mSrvDescriptorCount = _srvDescriptorCount;
	mUavDescriptorCount = _uavDescriptorCount;

	mShaderVisibleDescriptorHeap = _shaderVisibleDescriptorHeap;

	{
		mCPUDescriptorHandle.ptr =
			mShaderVisibleDescriptorHeap->GetSrvUavDescriptorHeapStartCPUDescriptorHandle().ptr +
			_srvUavDescriptorHeapIndex * mShaderVisibleDescriptorHeap->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		mGPUDescriptorHandle.ptr = 
			mShaderVisibleDescriptorHeap->GetSrvUavDescriptorHeapStartGPUDescriptorHandle().ptr +
			_srvUavDescriptorHeapIndex * mShaderVisibleDescriptorHeap->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}
}

void SD3D12DescriptorHeapRange::SetSRV(uint64_t _offset, IRDIShaderResourceView* _srv) noexcept
{
	CHECK(_offset < mSrvDescriptorCount);
	mShaderVisibleDescriptorHeap->SetSRV(_offset + GetSrvDescriptorHeapIndex(), _srv);
}

void SD3D12DescriptorHeapRange::SetUAV(uint64_t _offset, IRDIUnorderedAccessView* _uav) noexcept
{
	CHECK(_offset < mUavDescriptorCount);
	mShaderVisibleDescriptorHeap->SetUAV(_offset + GetUavDescriptorHeapIndex(), _uav);
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
		mCPUDescriptorHandle.ptr =
			mShaderVisibleDescriptorHeap->GetSrvUavDescriptorHeapStartCPUDescriptorHandle().ptr +
			_samplerViewDescriptorHeapIndex * mShaderVisibleDescriptorHeap->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

		mGPUDescriptorHandle.ptr =
			mShaderVisibleDescriptorHeap->GetSrvUavDescriptorHeapStartGPUDescriptorHandle().ptr +
			_samplerViewDescriptorHeapIndex * mShaderVisibleDescriptorHeap->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	}
}

void SD3D12SamplerHeapRange::SetSampler(uint64_t _offset, IRDISamplerView* _sampler) noexcept
{
	CHECK(_offset < mSamplerViewDescriptorCount);
	mShaderVisibleDescriptorHeap->SetSampler(_offset + mSamplerViewDescriptorHeapIndex, _sampler);
}

void SD3D12SamplerHeapRange::Release() noexcept
{
	mShaderVisibleDescriptorHeap->ReleaseSamplerHeapRange(this);
}


void SD3D12ShaderVisibleDescriptorHeap::Init(ID3D12DescriptorHeap* _srvUavDescriptorHeapNativePtr, uint64_t _srvUavDescriptorCount, ID3D12DescriptorHeap* _samplerViewDescriptorHeapNativePtr, uint64_t _samplerViewDescriptorCount, SD3D12Device* _device) noexcept
{
	mSrvUavDescriptorHeapNativePtr = _srvUavDescriptorHeapNativePtr;
	mSamplerViewDescriptorHeapNativePtr = _samplerViewDescriptorHeapNativePtr;

	mSrvUavDescriptorAlloctorHelper.ResetSlotCount(_srvUavDescriptorCount);
	mSamplerViewAlloctorHelper.ResetSlotCount(_samplerViewDescriptorCount);

	mSrvUavDescriptorHeapStartCPUDescriptorHandle = GetSrvUavDescriptorHeapNativePtr()->GetCPUDescriptorHandleForHeapStart();
	mSamplerViewDescriptorHeapStartCPUDescriptorHandle = GetSamplerViewDescriptorHeapNativePtr()->GetCPUDescriptorHandleForHeapStart();

	mSrvUavDescriptorHeapStartGPUDescriptorHandle = GetSrvUavDescriptorHeapNativePtr()->GetGPUDescriptorHandleForHeapStart();
	mSamplerViewDescriptorHeapStartGPUDescriptorHandle = GetSamplerViewDescriptorHeapNativePtr()->GetGPUDescriptorHandleForHeapStart();

	mDevice = _device;
}

SD3D12DescriptorHeapRange* SD3D12ShaderVisibleDescriptorHeap::AllocateDescriptorHeapRange(uint16_t _srvDescriptorCount, uint16_t _uavDescriptorCount) noexcept
{
	uint64_t srvUavStart = mSrvUavDescriptorAlloctorHelper.AllocateSlotArray(0ull + _srvDescriptorCount + _uavDescriptorCount);

	CHECK(srvUavStart != SCircularSlotArrayAllocatorHelper::ErrorSlot && L"描述符堆中没有充足的空间以为新资源分配描述符");

	SD3D12DescriptorHeapRange descriptorHeapRange;
	descriptorHeapRange.Init(srvUavStart, _srvDescriptorCount, _uavDescriptorCount, this);

	return mDescriptorHeapRangePool.AllocateElement(descriptorHeapRange);
}

SD3D12SamplerHeapRange* SD3D12ShaderVisibleDescriptorHeap::AllocateSamplerHeapRange(uint16_t _samplerViewDescriptorCount) noexcept
{
	uint64_t samplerViewStart = mSamplerViewAlloctorHelper.AllocateSlotArray(_samplerViewDescriptorCount);

	CHECK(samplerViewStart != SCircularSlotArrayAllocatorHelper::ErrorSlot && L"Sampler描述符堆中没有充足的空间以为新资源分配描述符");

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

void SD3D12ShaderVisibleDescriptorHeap::SetSRV(uint64_t _index, IRDIShaderResourceView* _srv) noexcept
{
	D3D12_CPU_DESCRIPTOR_HANDLE destDescriptorHeapStartCPUDescriptorHandle;
	destDescriptorHeapStartCPUDescriptorHandle.ptr = mSrvUavDescriptorHeapStartCPUDescriptorHandle.ptr + GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * _index;
	D3D12_CPU_DESCRIPTOR_HANDLE srcDescriptorHeapStartCPUDescriptorHandle = static_cast<SD3D12ShaderResourceView*>(_srv)->mCpuDescriptorHandle;

	mDevice->GetNativePtr()->CopyDescriptorsSimple(1, destDescriptorHeapStartCPUDescriptorHandle, srcDescriptorHeapStartCPUDescriptorHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void SD3D12ShaderVisibleDescriptorHeap::SetUAV(uint64_t _index, IRDIUnorderedAccessView* _uav) noexcept
{
	D3D12_CPU_DESCRIPTOR_HANDLE destDescriptorHeapStartCPUDescriptorHandle;
	destDescriptorHeapStartCPUDescriptorHandle.ptr = mSrvUavDescriptorHeapStartCPUDescriptorHandle.ptr + GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * _index;
	D3D12_CPU_DESCRIPTOR_HANDLE srcDescriptorHeapStartCPUDescriptorHandle = static_cast<SD3D12UnorderedAccessView*>(_uav)->mCpuDescriptorHandle;
	
	mDevice->GetNativePtr()->CopyDescriptorsSimple(1, destDescriptorHeapStartCPUDescriptorHandle, srcDescriptorHeapStartCPUDescriptorHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void SD3D12ShaderVisibleDescriptorHeap::SetSampler(uint64_t _index, IRDISamplerView* _sampler) noexcept
{
	D3D12_CPU_DESCRIPTOR_HANDLE destDescriptorHeapStartCPUDescriptorHandle;
	destDescriptorHeapStartCPUDescriptorHandle.ptr = mSrvUavDescriptorHeapStartCPUDescriptorHandle.ptr + GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER) * _index;
	D3D12_CPU_DESCRIPTOR_HANDLE srcDescriptorHeapStartCPUDescriptorHandle = static_cast<SD3D12SamplerView*>(_sampler)->mCpuDescriptorHandle;
	
	mDevice->GetNativePtr()->CopyDescriptorsSimple(1, destDescriptorHeapStartCPUDescriptorHandle, srcDescriptorHeapStartCPUDescriptorHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

uint32_t SD3D12ShaderVisibleDescriptorHeap::GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE _descriptorHeapType) noexcept
{
	return mDevice->GetDescriptorHandleIncrement(_descriptorHeapType);
}

void SD3D12DescriptorHeap::Init(ID3D12DescriptorHeap* _rtvDescriptorHeapNativePtr, uint64_t _rtvDescriptorCount, ID3D12DescriptorHeap* _dsvDescriptorHeapNativePtr, uint64_t _dsvDescriptorCount, ID3D12DescriptorHeap* _srvDescriptorHeapNativePtr, uint64_t _srvDescriptorCount, ID3D12DescriptorHeap* _uavDescriptorHeapNativePtr, uint64_t _uavDescriptorCount, ID3D12DescriptorHeap* _samplerViewDescriptorHeapNativePtr, uint64_t _samplerViewDescriptorCount, SD3D12Device* _device) noexcept
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

	mRenderTargetViewPool.RecapacityPool(_rtvDescriptorCount);
	mDepthStencilViewPool.RecapacityPool(_dsvDescriptorCount);
	mShaderResourceViewPool.RecapacityPool(_srvDescriptorCount);
	mUnorderedAccessViewPool.RecapacityPool(_uavDescriptorCount);
	mSamplerViewPool.RecapacityPool(_samplerViewDescriptorCount);

	mRTVDescriptorHeapStartCPUDescriptorHandle = mRTVDescriptorHeapNativePtr->GetCPUDescriptorHandleForHeapStart();
	mDSVDescriptorHeapStartCPUDescriptorHandle = mDSVDescriptorHeapNativePtr->GetCPUDescriptorHandleForHeapStart();
	mSRVDescriptorHeapStartCPUDescriptorHandle = mSRVDescriptorHeapNativePtr->GetCPUDescriptorHandleForHeapStart();
	mUAVDescriptorHeapStartCPUDescriptorHandle = mUAVDescriptorHeapNativePtr->GetCPUDescriptorHandleForHeapStart();
	mSamplerViewDescriptorHeapStartCPUDescriptorHandle = mSamplerViewDescriptorHeapNativePtr->GetCPUDescriptorHandleForHeapStart();

	mRTVDescriptorHeapStartGPUDescriptorHandle = mRTVDescriptorHeapNativePtr->GetGPUDescriptorHandleForHeapStart();
	mDSVDescriptorHeapStartGPUDescriptorHandle = mDSVDescriptorHeapNativePtr->GetGPUDescriptorHandleForHeapStart();
	mSRVDescriptorHeapStartGPUDescriptorHandle = mSRVDescriptorHeapNativePtr->GetGPUDescriptorHandleForHeapStart();
	mUAVDescriptorHeapStartGPUDescriptorHandle = mUAVDescriptorHeapNativePtr->GetGPUDescriptorHandleForHeapStart();
	mSamplerViewDescriptorHeapStartGPUDescriptorHandle = mSamplerViewDescriptorHeapNativePtr->GetGPUDescriptorHandleForHeapStart();

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
	uint64_t index = (_rtv->mCpuDescriptorHandle.ptr - mRTVDescriptorHeapStartCPUDescriptorHandle.ptr) / mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	mRTVAllocatorHelper.DeallocateSlot(index);
	mRenderTargetViewPool.DeallocateElement(_rtv);
}

void SD3D12DescriptorHeap::ReleaseDSV(SD3D12DepthStencilView* _dsv) noexcept
{
	uint64_t index = (_dsv->mCpuDescriptorHandle.ptr - mDSVDescriptorHeapStartCPUDescriptorHandle.ptr) / mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	mDSVAllocatorHelper.DeallocateSlot(index);
	mDepthStencilViewPool.DeallocateElement(_dsv);
}

void SD3D12DescriptorHeap::ReleaseSRV(SD3D12ShaderResourceView* _srv) noexcept
{
	uint64_t index = (_srv->mCpuDescriptorHandle.ptr - mSRVDescriptorHeapStartCPUDescriptorHandle.ptr) / mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	mSRVAllocatorHelper.DeallocateSlot(index);
	mShaderResourceViewPool.DeallocateElement(_srv);
}

void SD3D12DescriptorHeap::ReleaseUAV(SD3D12UnorderedAccessView* _uav) noexcept
{
	uint64_t index = (_uav->mCpuDescriptorHandle.ptr - mUAVDescriptorHeapStartCPUDescriptorHandle.ptr) / mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	mUAVAllocatorHelper.DeallocateSlot(index);
	mUnorderedAccessViewPool.DeallocateElement(_uav);
}

void SD3D12DescriptorHeap::ReleaseSamplerView(SD3D12SamplerView* _samplerView) noexcept
{
	uint64_t index = (_samplerView->mCpuDescriptorHandle.ptr - mSamplerViewDescriptorHeapStartCPUDescriptorHandle.ptr) / mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

	mSamplerViewAllocatorHelper.DeallocateSlot(index);
	mSamplerViewPool.DeallocateElement(_samplerView);
}

SD3D12RenderTargetView* SD3D12DescriptorHeap::CreateRTV(ID3D12Resource* _resourceNativePtr, const D3D12_RENDER_TARGET_VIEW_DESC* _desc) noexcept
{
	uint64_t slotIndex = mRTVAllocatorHelper.AllocateSlot();

	CHECK(slotIndex != SCircularSlotAllocatorHelper::ErrorSlot);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	cpuHandle.ptr = mRTVDescriptorHeapStartCPUDescriptorHandle.ptr + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	gpuHandle.ptr = mRTVDescriptorHeapStartGPUDescriptorHandle.ptr + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	mDevice->GetNativePtr()->CreateRenderTargetView(_resourceNativePtr, _desc, cpuHandle);

	return mRenderTargetViewPool.AllocateElement(cpuHandle, gpuHandle);
}

SD3D12DepthStencilView* SD3D12DescriptorHeap::CreateDSV(ID3D12Resource* _resourceNativePtr, const D3D12_DEPTH_STENCIL_VIEW_DESC* _desc) noexcept
{
	uint64_t slotIndex = mDSVAllocatorHelper.AllocateSlot();

	CHECK(slotIndex != SCircularSlotAllocatorHelper::ErrorSlot);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	cpuHandle.ptr = mDSVDescriptorHeapStartCPUDescriptorHandle.ptr + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	gpuHandle.ptr = mDSVDescriptorHeapStartGPUDescriptorHandle.ptr + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	mDevice->GetNativePtr()->CreateDepthStencilView(_resourceNativePtr, _desc, cpuHandle);

	return mDepthStencilViewPool.AllocateElement(cpuHandle, gpuHandle);
}

SD3D12ShaderResourceView* SD3D12DescriptorHeap::CreateSRV(ID3D12Resource* _resourceNativePtr, const D3D12_SHADER_RESOURCE_VIEW_DESC* _desc) noexcept
{
	uint64_t slotIndex = mSRVAllocatorHelper.AllocateSlot();

	CHECK(slotIndex != SCircularSlotAllocatorHelper::ErrorSlot);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	cpuHandle.ptr = mSRVDescriptorHeapStartCPUDescriptorHandle.ptr + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	gpuHandle.ptr = mSRVDescriptorHeapStartGPUDescriptorHandle.ptr + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	mDevice->GetNativePtr()->CreateShaderResourceView(_resourceNativePtr, _desc, cpuHandle);

	return mShaderResourceViewPool.AllocateElement(cpuHandle, gpuHandle);
}

SD3D12UnorderedAccessView* SD3D12DescriptorHeap::CreateUAV(ID3D12Resource* _resourceNativePtr, const D3D12_UNORDERED_ACCESS_VIEW_DESC* _desc) noexcept
{
	uint64_t slotIndex = mUAVAllocatorHelper.AllocateSlot();

	CHECK(slotIndex != SCircularSlotAllocatorHelper::ErrorSlot);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	cpuHandle.ptr = mUAVDescriptorHeapStartCPUDescriptorHandle.ptr + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	gpuHandle.ptr = mUAVDescriptorHeapStartGPUDescriptorHandle.ptr + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	mDevice->GetNativePtr()->CreateUnorderedAccessView(_resourceNativePtr, nullptr, _desc, cpuHandle);

	return mUnorderedAccessViewPool.AllocateElement(cpuHandle, gpuHandle);
}

SD3D12SamplerView* SD3D12DescriptorHeap::CreateSamplerView(const D3D12_SAMPLER_DESC* _desc) noexcept
{
	uint64_t slotIndex = mSamplerViewAllocatorHelper.AllocateSlot();

	CHECK(slotIndex != SCircularSlotAllocatorHelper::ErrorSlot);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	cpuHandle.ptr = mSamplerViewDescriptorHeapStartCPUDescriptorHandle.ptr + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	gpuHandle.ptr = mSamplerViewDescriptorHeapStartGPUDescriptorHandle.ptr + slotIndex * mDevice->GetDescriptorHandleIncrement(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

	mDevice->GetNativePtr()->CreateSampler(_desc, cpuHandle);

	return mSamplerViewPool.AllocateElement(cpuHandle, gpuHandle);
}

#include "D3D/D3DUtil.h"
#include "D3D12Resource.h"
#include "Core/Macros/Assert.h"
#include "D3D/D3D12/D3D12Device.h"
#include "D3D/D3D12/Warper/D3D12ImplWarper.h"

void SD3D12Buffer::Init(void* _nativePtr, const SRDIBufferResourceDesc* _desc, SD3D12Device* _device) noexcept
{
	mResourceNativePtr = _nativePtr;
	mDesc = *_desc;

	SD3D12DescriptorHeap* descriptorHeap = _device->GetDescriptorHeap();

	D3D12APIWarp_Impl::D3D12GetGPUVirtualAddress_D3D12Impl(GetNativePtr(), &mGpuVirtualAddress);
	
	if ((mDesc.mResourceUsage & ERDIResourceUsage::IndexBuffer) == ERDIResourceUsage::IndexBuffer)
	{
		mIBV.mGpuVirtualAddress = mGpuVirtualAddress;
		mIBV.mSizeInByte = static_cast<uint32_t>(mDesc.mBufferSize);
		switch (mDesc.mElementStride)
		{
		case 2:
			mIBV.mFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
			break;
		case 4:
			mIBV.mFormat = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
			break;
		default:
			CHECK(false);
		}
	}

	if ((mDesc.mResourceUsage & ERDIResourceUsage::VertexBuffer) == ERDIResourceUsage::VertexBuffer)
	{
		mVBV.mGpuVirtualAddress = mGpuVirtualAddress;
		mVBV.mSizeInByte = static_cast<uint32_t>(mDesc.mBufferSize);
		mVBV.mStrideInBytes = mDesc.mElementStride;
	}

	if ((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource)
		mSRV = descriptorHeap->CreateSRV(this);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess)
		mUAV = descriptorHeap->CreateUAV(this);
}

IRDIIndexBufferView* SD3D12Buffer::GetIBV() noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::IndexBuffer) == ERDIResourceUsage::IndexBuffer);
	return &mIBV;
}

IRDIVertexBufferView* SD3D12Buffer::GetVBV() noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::VertexBuffer) == ERDIResourceUsage::VertexBuffer);
	return &mVBV;
}

IRDIShaderResourceView* SD3D12Buffer::GetSRV() noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource);
	return mSRV;
}

IRDIUnorderedAccessView* SD3D12Buffer::GetUAV() noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess);
	return mUAV;
}

void SD3D12Texture1D::Init(void* _nativePtr, const SRDITexture1DResourceDesc* _desc, SD3D12Device* _device) noexcept
{
	mResourceNativePtr = _nativePtr;
	mDesc = *_desc;

	SD3D12DescriptorHeap* descriptorHeap = _device->GetDescriptorHeap();

	CHECK(mDesc.mMipCount != 0);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::IndexBuffer) != ERDIResourceUsage::IndexBuffer);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::VertexBuffer) != ERDIResourceUsage::VertexBuffer);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::ConstantBuffer) != ERDIResourceUsage::ConstantBuffer);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::RenderTarget) == ERDIResourceUsage::RenderTarget)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			mRTVs[i] = descriptorHeap->CreateRTV(this, i);
		}
	}

	if ((mDesc.mResourceUsage & ERDIResourceUsage::DepthStencil) == ERDIResourceUsage::DepthStencil)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			mDSVs[i] = descriptorHeap->CreateDSV(this, i);
		}
	}

	if ((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource)
		mSRV = descriptorHeap->CreateSRV(this);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			mUAVs[i] = descriptorHeap->CreateUAV(this, i);
		}
	}
}

IRDIRenderTargetView* SD3D12Texture1D::GetRTV(uint32_t _mipSlice) noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::RenderTarget) == ERDIResourceUsage::RenderTarget);
	CHECK(_mipSlice < mDesc.mMipCount);

	return mRTVs[_mipSlice];
}

IRDIDepthStencilView* SD3D12Texture1D::GetDSV(uint32_t _mipSlice) noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::DepthStencil) == ERDIResourceUsage::DepthStencil);
	CHECK(_mipSlice < mDesc.mMipCount);

	return mDSVs[_mipSlice];
}

IRDIShaderResourceView* SD3D12Texture1D::GetSRV() noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource);
	return mSRV;
}

IRDIUnorderedAccessView* SD3D12Texture1D::GetUAV(uint32_t _mipSlice) noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess);
	CHECK(_mipSlice < mDesc.mMipCount);

	return mUAVs[_mipSlice];
}

void SD3D12Texture1DArray::Init(void* _nativePtr, const SRDITexture1DArrayResourceDesc* _desc, SD3D12Device* _device) noexcept
{
	mResourceNativePtr = _nativePtr;
	mDesc = *_desc;

	SD3D12DescriptorHeap* descriptorHeap = _device->GetDescriptorHeap();

	CHECK(mDesc.mMipCount != 0);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::IndexBuffer) != ERDIResourceUsage::IndexBuffer);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::VertexBuffer) != ERDIResourceUsage::VertexBuffer);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::RenderTarget) != ERDIResourceUsage::RenderTarget);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::DepthStencil) != ERDIResourceUsage::DepthStencil);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::ConstantBuffer) != ERDIResourceUsage::ConstantBuffer);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource)
		mSRV = descriptorHeap->CreateSRV(this);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			mUAVs[i] = descriptorHeap->CreateUAV(this, i);
		}
	}
}

IRDIShaderResourceView* SD3D12Texture1DArray::GetSRV() noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource);
	return mSRV;
}

IRDIUnorderedAccessView* SD3D12Texture1DArray::GetUAV(uint32_t _mipSlice) noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess);
	CHECK(_mipSlice < mDesc.mMipCount);

	return mUAVs[_mipSlice];
}

void SD3D12Texture2D::Init(void* _nativePtr, const SRDITexture2DResourceDesc* _desc, SD3D12Device* _device) noexcept
{
	mResourceNativePtr = _nativePtr;
	mDesc = *_desc;

	SD3D12DescriptorHeap* descriptorHeap = _device->GetDescriptorHeap();

	CHECK(mDesc.mMipCount != 0);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::IndexBuffer) != ERDIResourceUsage::IndexBuffer);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::VertexBuffer) != ERDIResourceUsage::VertexBuffer);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::ConstantBuffer) != ERDIResourceUsage::ConstantBuffer);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::RenderTarget) == ERDIResourceUsage::RenderTarget)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			mRTVs[i] = descriptorHeap->CreateRTV(this, i);
		}
	}

	if ((mDesc.mResourceUsage & ERDIResourceUsage::DepthStencil) == ERDIResourceUsage::DepthStencil)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			mDSVs[i] = descriptorHeap->CreateDSV(this, i);
		}
	}

	if ((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource)
		mSRV = descriptorHeap->CreateSRV(this);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			mUAVs[i] = descriptorHeap->CreateUAV(this, i);
		}
	}
}

IRDIRenderTargetView* SD3D12Texture2D::GetRTV(uint32_t _mipSlice) noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::RenderTarget) == ERDIResourceUsage::RenderTarget);
	CHECK(_mipSlice < mDesc.mMipCount);

	return mRTVs[_mipSlice];
}

IRDIDepthStencilView* SD3D12Texture2D::GetDSV(uint32_t _mipSlice) noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::DepthStencil) == ERDIResourceUsage::DepthStencil);
	CHECK(_mipSlice < mDesc.mMipCount);

	return mDSVs[_mipSlice];
}

IRDIShaderResourceView* SD3D12Texture2D::GetSRV() noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource);
	return mSRV;
}

IRDIUnorderedAccessView* SD3D12Texture2D::GetUAV(uint32_t _mipSlice) noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess);
	CHECK(_mipSlice < mDesc.mMipCount);

	return mUAVs[_mipSlice];
}

void SD3D12Texture2DArray::Init(void* _nativePtr, const SRDITexture2DArrayResourceDesc* _desc, SD3D12Device* _device) noexcept
{
	mResourceNativePtr = _nativePtr;
	mDesc = *_desc;

	SD3D12DescriptorHeap* descriptorHeap = _device->GetDescriptorHeap();

	CHECK(mDesc.mMipCount != 0);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::IndexBuffer) != ERDIResourceUsage::IndexBuffer);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::VertexBuffer) != ERDIResourceUsage::VertexBuffer);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::RenderTarget) != ERDIResourceUsage::RenderTarget);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::DepthStencil) != ERDIResourceUsage::DepthStencil);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::ConstantBuffer) != ERDIResourceUsage::ConstantBuffer);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource)
		mSRV = descriptorHeap->CreateSRV(this);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			mUAVs[i] = descriptorHeap->CreateUAV(this, i);
		}
	}
}

IRDIShaderResourceView* SD3D12Texture2DArray::GetSRV() noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource);
	return mSRV;
}

IRDIUnorderedAccessView* SD3D12Texture2DArray::GetUAV(uint32_t _mipSlice) noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess);
	CHECK(_mipSlice < mDesc.mMipCount);

	return mUAVs[_mipSlice];
}

void SD3D12Texture3D::Init(void* _nativePtr, const SRDITexture3DResourceDesc* _desc, SD3D12Device* _device) noexcept
{
	mResourceNativePtr = _nativePtr;
	mDesc = *_desc;

	SD3D12DescriptorHeap* descriptorHeap = _device->GetDescriptorHeap();

	CHECK(mDesc.mMipCount != 0);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::IndexBuffer) != ERDIResourceUsage::IndexBuffer);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::VertexBuffer) != ERDIResourceUsage::VertexBuffer);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::ConstantBuffer) != ERDIResourceUsage::ConstantBuffer);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::DepthStencil) != ERDIResourceUsage::DepthStencil);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::RenderTarget) == ERDIResourceUsage::RenderTarget)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			mRTVs[i] = descriptorHeap->CreateRTV(this, i);
		}
	}

	if ((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource)
		mSRV = descriptorHeap->CreateSRV(this);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			mUAVs[i] = descriptorHeap->CreateUAV(this, i);
		}
	}
}

IRDIRenderTargetView* SD3D12Texture3D::GetRTV(uint32_t _mipSlice) noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::RenderTarget) == ERDIResourceUsage::RenderTarget);
	CHECK(_mipSlice < mDesc.mMipCount);

	return mRTVs[_mipSlice];
}

IRDIShaderResourceView* SD3D12Texture3D::GetSRV() noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource);
	return mSRV;
}

IRDIUnorderedAccessView* SD3D12Texture3D::GetUAV(uint32_t _mipSlice) noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess);
	CHECK(_mipSlice < mDesc.mMipCount);

	return mUAVs[_mipSlice];
}

void SD3D12TextureCube::Init(void* _nativePtr, const SRDITextureCubeResourceDesc* _desc, SD3D12Device* _device) noexcept
{
	mResourceNativePtr = _nativePtr;
	mDesc = *_desc;

	SD3D12DescriptorHeap* descriptorHeap = _device->GetDescriptorHeap();

	CHECK(mDesc.mMipCount != 0);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::IndexBuffer) != ERDIResourceUsage::IndexBuffer);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::VertexBuffer) != ERDIResourceUsage::VertexBuffer);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::ConstantBuffer) != ERDIResourceUsage::ConstantBuffer);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::RenderTarget) == ERDIResourceUsage::RenderTarget)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			for (size_t cubeFace = 0; cubeFace != EnumToInt(ERDITextureCubeFace::FaceCount); ++cubeFace)
				mRTVs[cubeFace + 1ull * i * EnumToInt(ERDITextureCubeFace::FaceCount)] = descriptorHeap->CreateRTV(this, static_cast<ERDITextureCubeFace>(cubeFace), i);
		}
	}

	if ((mDesc.mResourceUsage & ERDIResourceUsage::DepthStencil) == ERDIResourceUsage::DepthStencil)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			for (size_t cubeFace = 0; cubeFace != EnumToInt(ERDITextureCubeFace::FaceCount); ++cubeFace)
				mDSVs[cubeFace + 1ull * i * EnumToInt(ERDITextureCubeFace::FaceCount)] = descriptorHeap->CreateDSV(this, static_cast<ERDITextureCubeFace>(cubeFace), i);
		}
	}

	if ((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource)
		mSRV = descriptorHeap->CreateSRV(this);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			mUAVs[i] = descriptorHeap->CreateUAV(this, i);
		}
	}
}

IRDIRenderTargetView* SD3D12TextureCube::GetRTV(ERDITextureCubeFace _face, uint32_t _mipSlice) noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::RenderTarget) == ERDIResourceUsage::RenderTarget);
	CHECK(_mipSlice < mDesc.mMipCount);

	return mRTVs[EnumToInt(_face) + 1ull * _mipSlice * EnumToInt(ERDITextureCubeFace::FaceCount)];
}

IRDIDepthStencilView* SD3D12TextureCube::GetDSV(ERDITextureCubeFace _face, uint32_t _mipSlice) noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::DepthStencil) == ERDIResourceUsage::DepthStencil);
	CHECK(_mipSlice < mDesc.mMipCount);

	return mDSVs[EnumToInt(_face) + 1ull * _mipSlice * EnumToInt(ERDITextureCubeFace::FaceCount)];
}

IRDIShaderResourceView* SD3D12TextureCube::GetSRV() noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource);
	return mSRV;
}

IRDIUnorderedAccessView* SD3D12TextureCube::GetUAV(uint32_t _mipSlice) noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess);
	CHECK(_mipSlice < mDesc.mMipCount);

	return mUAVs[_mipSlice];
}

void SD3D12TextureCubeArray::Init(void* _nativePtr, const SRDITextureCubeArrayResourceDesc* _desc, SD3D12Device* _device) noexcept
{
	mResourceNativePtr = _nativePtr;
	mDesc = *_desc;

	SD3D12DescriptorHeap* descriptorHeap = _device->GetDescriptorHeap();

	CHECK(mDesc.mMipCount != 0);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::IndexBuffer) != ERDIResourceUsage::IndexBuffer);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::VertexBuffer) != ERDIResourceUsage::VertexBuffer);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::RenderTarget) != ERDIResourceUsage::RenderTarget);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::DepthStencil) != ERDIResourceUsage::DepthStencil);
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::ConstantBuffer) != ERDIResourceUsage::ConstantBuffer);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource)
		mSRV = descriptorHeap->CreateSRV(this);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			mUAVs[i] = descriptorHeap->CreateUAV(this, i);
		}
	}
}

IRDIShaderResourceView* SD3D12TextureCubeArray::GetSRV() noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource);
	return mSRV;
}

IRDIUnorderedAccessView* SD3D12TextureCubeArray::GetUAV(uint32_t _mipSlice) noexcept
{
	CHECK((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess);
	CHECK(_mipSlice < mDesc.mMipCount);

	return mUAVs[_mipSlice];
}

void SD3D12Sampler::Init(const SRDISamplerResourceDesc* _desc, SD3D12Device* _device) noexcept
{
	mDesc = *_desc;

	SD3D12DescriptorHeap* descriptorHeap = _device->GetDescriptorHeap();

	mSamplerView = descriptorHeap->CreateSamplerView(this);
}

IRDISamplerView* SD3D12Sampler::GetSamplerView() noexcept
{
	return mSamplerView;
}

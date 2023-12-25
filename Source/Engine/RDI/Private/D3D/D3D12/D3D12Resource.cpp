#include "D3D/D3DUtil.h"
#include "D3D12Resource.h"
#include "Core/Util/Assert.h"
#include "D3D/D3D12/D3D12Device.h"

void SD3D12Buffer::Init(ID3D12Resource* _nativePtr, const SRDIBufferResourceDesc* _desc, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mResourceNativePtr = _nativePtr;
	mDesc = *_desc;

	SD3D12DescriptorHeap* descriptorHeap = mDevice->GetDescriptorHeap();

	mGpuVirtualAddress = GetNativePtr()->GetGPUVirtualAddress();
	
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

SBufferView SD3D12Buffer::Map() noexcept
{
	CHECK(mDesc.mHeapType == ERDIHeapType::Upload || mDesc.mHeapType == ERDIHeapType::ReadBack);

	void* dataPtr = nullptr;
	D3D12_RANGE readRange(0, 0);

	VERIFY_D3D_RETURN(mResourceNativePtr->Map(0, &readRange, &dataPtr));
	return SBufferView(dataPtr, mDesc.mBufferSize);
}

void SD3D12Buffer::Unmap() noexcept
{
	D3D12_RANGE range(0, 0);
	mResourceNativePtr->Unmap(0, &range);
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

void SD3D12Buffer::Release() noexcept
{
	SD3D12DescriptorHeap* descriptorHeap = mDevice->GetDescriptorHeap();

	if ((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource)
		descriptorHeap->ReleaseSRV(mSRV);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess)
		descriptorHeap->ReleaseUAV(mUAV);

	mDevice->ReleaseBuffer(this);
}

void SD3D12Texture1D::Init(ID3D12Resource* _nativePtr, const SRDITexture1DResourceDesc* _desc, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mResourceNativePtr = _nativePtr;
	mDesc = *_desc;

	SD3D12DescriptorHeap* descriptorHeap = mDevice->GetDescriptorHeap();

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

void SD3D12Texture1D::Release() noexcept
{
	SD3D12DescriptorHeap* descriptorHeap = mDevice->GetDescriptorHeap();

	if ((mDesc.mResourceUsage & ERDIResourceUsage::RenderTarget) == ERDIResourceUsage::RenderTarget)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			descriptorHeap->ReleaseRTV(mRTVs[i]);
		}
	}

	if ((mDesc.mResourceUsage & ERDIResourceUsage::DepthStencil) == ERDIResourceUsage::DepthStencil)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			descriptorHeap->ReleaseDSV(mDSVs[i]);
		}
	}

	if ((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource)
		descriptorHeap->ReleaseSRV(mSRV);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			descriptorHeap->ReleaseUAV(mUAVs[i]);
		}
	}

	mDevice->ReleaseTexture1D(this);
}

void SD3D12Texture1DArray::Init(ID3D12Resource* _nativePtr, const SRDITexture1DArrayResourceDesc* _desc, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mResourceNativePtr = _nativePtr;
	mDesc = *_desc;

	SD3D12DescriptorHeap* descriptorHeap = mDevice->GetDescriptorHeap();

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

void SD3D12Texture1DArray::Release() noexcept
{
	SD3D12DescriptorHeap* descriptorHeap = mDevice->GetDescriptorHeap();

	if ((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource)
		descriptorHeap->ReleaseSRV(mSRV);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			descriptorHeap->ReleaseUAV(mUAVs[i]);
		}
	}

	mDevice->ReleaseTexture1DArray(this);
}

void SD3D12Texture2D::Init(ID3D12Resource* _nativePtr, const SRDITexture2DResourceDesc* _desc, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mResourceNativePtr = _nativePtr;
	mDesc = *_desc;

	SD3D12DescriptorHeap* descriptorHeap = mDevice->GetDescriptorHeap();

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

void SD3D12Texture2D::Release() noexcept
{
	SD3D12DescriptorHeap* descriptorHeap = mDevice->GetDescriptorHeap();

	if ((mDesc.mResourceUsage & ERDIResourceUsage::RenderTarget) == ERDIResourceUsage::RenderTarget)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			descriptorHeap->ReleaseRTV(mRTVs[i]);
		}
	}

	if ((mDesc.mResourceUsage & ERDIResourceUsage::DepthStencil) == ERDIResourceUsage::DepthStencil)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			descriptorHeap->ReleaseDSV(mDSVs[i]);
		}
	}

	if ((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource)
		descriptorHeap->ReleaseSRV(mSRV);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			descriptorHeap->ReleaseUAV(mUAVs[i]);
		}
	}

	mDevice->ReleaseTexture2D(this);
}

void SD3D12Texture2DArray::Init(ID3D12Resource* _nativePtr, const SRDITexture2DArrayResourceDesc* _desc, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mResourceNativePtr = _nativePtr;
	mDesc = *_desc;

	SD3D12DescriptorHeap* descriptorHeap = mDevice->GetDescriptorHeap();

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

void SD3D12Texture2DArray::Release() noexcept
{
	SD3D12DescriptorHeap* descriptorHeap = mDevice->GetDescriptorHeap();

	if ((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource)
		descriptorHeap->ReleaseSRV(mSRV);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			descriptorHeap->ReleaseUAV(mUAVs[i]);
		}
	}

	mDevice->ReleaseTexture2DArray(this);
}

void SD3D12Texture3D::Init(ID3D12Resource* _nativePtr, const SRDITexture3DResourceDesc* _desc, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mResourceNativePtr = _nativePtr;
	mDesc = *_desc;

	SD3D12DescriptorHeap* descriptorHeap = mDevice->GetDescriptorHeap();

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

void SD3D12Texture3D::Release() noexcept
{
	SD3D12DescriptorHeap* descriptorHeap = mDevice->GetDescriptorHeap();

	if ((mDesc.mResourceUsage & ERDIResourceUsage::RenderTarget) == ERDIResourceUsage::RenderTarget)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			descriptorHeap->ReleaseRTV(mRTVs[i]);
		}
	}

	if ((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource)
		descriptorHeap->ReleaseSRV(mSRV);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			descriptorHeap->ReleaseUAV(mUAVs[i]);
		}
	}

	mDevice->ReleaseTexture3D(this);
}

void SD3D12TextureCube::Init(ID3D12Resource* _nativePtr, const SRDITextureCubeResourceDesc* _desc, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mResourceNativePtr = _nativePtr;
	mDesc = *_desc;

	SD3D12DescriptorHeap* descriptorHeap = mDevice->GetDescriptorHeap();

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

void SD3D12TextureCube::Release() noexcept
{
	SD3D12DescriptorHeap* descriptorHeap = mDevice->GetDescriptorHeap();

	if ((mDesc.mResourceUsage & ERDIResourceUsage::RenderTarget) == ERDIResourceUsage::RenderTarget)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			for (size_t cubeFace = 0; cubeFace != EnumToInt(ERDITextureCubeFace::FaceCount); ++cubeFace)
				descriptorHeap->ReleaseRTV(mRTVs[cubeFace + 1ull * i * EnumToInt(ERDITextureCubeFace::FaceCount)]);
		}
	}

	if ((mDesc.mResourceUsage & ERDIResourceUsage::DepthStencil) == ERDIResourceUsage::DepthStencil)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			for (size_t cubeFace = 0; cubeFace != EnumToInt(ERDITextureCubeFace::FaceCount); ++cubeFace)
				descriptorHeap->ReleaseDSV(mDSVs[cubeFace + 1ull * i * EnumToInt(ERDITextureCubeFace::FaceCount)]);
		}
	}

	if ((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource)
		descriptorHeap->ReleaseSRV(mSRV);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			descriptorHeap->ReleaseUAV(mUAVs[i]);
		}
	}

	mDevice->ReleaseTextureCube(this);
}

void SD3D12TextureCubeArray::Init(ID3D12Resource* _nativePtr, const SRDITextureCubeArrayResourceDesc* _desc, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mResourceNativePtr = _nativePtr;
	mDesc = *_desc;

	SD3D12DescriptorHeap* descriptorHeap = mDevice->GetDescriptorHeap();

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

void SD3D12TextureCubeArray::Release() noexcept
{
	SD3D12DescriptorHeap* descriptorHeap = mDevice->GetDescriptorHeap();

	if ((mDesc.mResourceUsage & ERDIResourceUsage::ShaderResource) == ERDIResourceUsage::ShaderResource)
		descriptorHeap->ReleaseSRV(mSRV);

	if ((mDesc.mResourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess)
	{
		for (uint32_t i = 0; i != mDesc.mMipCount; ++i)
		{
			descriptorHeap->ReleaseUAV(mUAVs[i]);
		}
	}

	mDevice->ReleaseTextureCubeArray(this);
}

void SD3D12Sampler::Init(const SRDISamplerResourceDesc* _desc, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mDesc = *_desc;

	SD3D12DescriptorHeap* descriptorHeap = mDevice->GetDescriptorHeap();

	mSamplerView = descriptorHeap->CreateSamplerView(this);
}

IRDISamplerView* SD3D12Sampler::GetSamplerView() noexcept
{
	return mSamplerView;
}

void SD3D12Sampler::Release() noexcept
{
	SD3D12DescriptorHeap* descriptorHeap = mDevice->GetDescriptorHeap();

	descriptorHeap->ReleaseSamplerView(mSamplerView);

	mDevice->ReleaseSampler(this);
}

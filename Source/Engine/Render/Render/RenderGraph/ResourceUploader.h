 #pragma once
#include "Core/Util/Assert.h"
#include "RDI/RDITypeTraits.h"
#include "Render/RenderModule.h"
#include "Core/Util/UtilMacros.h"
#include "RDI/Interface/RDIDevice.h"
#include "RDI/Interface/RDIResource.h"
#include "RDI/Interface/RDICommandList.h"

#include <concepts>
#include <algorithm>

__interface IRDICommandList;

DECLARE_NAMED_CONCEPT_WITH_MEMBER(CDescWithSizeX, mSizeX)
DECLARE_NAMED_CONCEPT_WITH_MEMBER(CDescWithSizeY, mSizeY)
DECLARE_NAMED_CONCEPT_WITH_MEMBER(CDescWithSizeXY, mSizeXY)
DECLARE_NAMED_CONCEPT_WITH_MEMBER(CDescWithSizeZ, mSizeZ)
DECLARE_NAMED_CONCEPT_WITH_MEMBER(CDescWithArraySize, mArraySize)
DECLARE_NAMED_CONCEPT_WITH_MEMBER(CDescWithMipCount, mMipCount)

template<std::derived_from<IRDIResource> _rdiResourceType>
class TDynamicGPUResrouce
{
public:
	using RDIResourceType = _rdiResourceType;
	using RDIResourceDescType = TRDIResourceDescType<_rdiResourceType>;

	static TDynamicGPUResrouce ConstructDynamicGPUResource(IRDIDevice* _device, const RDIResourceDescType* _desc)
	{
		RDIResourceType* rdiResource = nullptr;

		if constexpr (std::is_same_v<RDIResourceType, IRDIBuffer>)
			rdiResource = _device->CreateBuffer(_desc);
		else if constexpr (std::is_same_v<RDIResourceType, IRDITexture1D>)
			rdiResource = _device->CreateTexture1D(_desc);
		else if constexpr (std::is_same_v<RDIResourceType, IRDITexture1DArray>)
			rdiResource = _device->CreateTexture1DArray(_desc);
		else if constexpr (std::is_same_v<RDIResourceType, IRDITexture2D>)
			rdiResource = _device->CreateTexture2D(_desc);
		else if constexpr (std::is_same_v<RDIResourceType, IRDITexture2DArray>)
			rdiResource = _device->CreateTexture2DArray(_desc);
		else if constexpr (std::is_same_v<RDIResourceType, IRDITexture3D>)
			rdiResource = _device->CreateTexture3D(_desc);
		else if constexpr (std::is_same_v<RDIResourceType, IRDITextureCube>)
			rdiResource = _device->CreateTextureCube(_desc);
		else if constexpr (std::is_same_v<RDIResourceType, IRDITextureCubeArray>)
			rdiResource = _device->CreateTextureCubeArray(_desc);

		return TDynamicGPUResrouce(_device, rdiResource);
	}

	TDynamicGPUResrouce() noexcept = default;
	TDynamicGPUResrouce(const TDynamicGPUResrouce&) noexcept = delete;

	TDynamicGPUResrouce(IRDIDevice* _device, RDIResourceType* _rdiResource) noexcept
		: mRDIDevice(_device), mRDIResource(_rdiResource)
	{
	}

	TDynamicGPUResrouce(TDynamicGPUResrouce&& _resource) noexcept
	{
		mRDIDevice = _resource.mRDIDevice;
		mRDIResource = _resource.mRDIResource;

		for (size_t i = 0; i != GRenderInfoCount; ++i)
			mUploadBuffer[i] = _resource.mUploadBuffer[i];

		_resource.mRDIResource = nullptr;
		for (size_t i = 0; i != GRenderInfoCount; ++i)
			_resource.mUploadBuffer[i] = nullptr;

		_resource.mRDIDevice = nullptr;
		_resource.mRDIResource = nullptr;
	}

	~TDynamicGPUResrouce() noexcept
	{
		if (mRDIResource)
			mRDIResource->Release();

		for (size_t i = 0; i != GRenderInfoCount; ++i)
			if (mUploadBuffer[i])
				mUploadBuffer[i]->Release();
	}

	void operator= (TDynamicGPUResrouce&& _resource) noexcept
	{
		new(this) TDynamicGPUResrouce(std::move(_resource));
		_resource.~TDynamicGPUResrouce();
	}

	bool IsValid() const noexcept { return mRDIResource != nullptr; }
	RDIResourceType* GetRDIResource() noexcept { return mRDIResource; }
	RDIResourceDescType GetDesc() noexcept
	{
		if (IsValid())
		{
			RDIResourceDescType desc;
			GetRDIResource()->GetDesc(&desc);
			return desc;
		}
		else
		{
			return RDIResourceDescType();
		}
	}

	SBufferView Map() noexcept
	{
		CHECK(mRDIResource && mRDIDevice);

		if (mUploadBuffer[GetUploadBufferIndex()] == nullptr)
			mUploadBuffer[GetUploadBufferIndex()] = CreateUploadBuffer();

		return mUploadBuffer[GetUploadBufferIndex()]->Map();
	}

	void Unmap() noexcept
	{
		CHECK(mRDIResource && mRDIDevice);

		mUploadBuffer[GetUploadBufferIndex()]->Unmap();
	}

	void Upload(IRDICommandList* _commandList) noexcept
	{
		IRDIBuffer* uploadBuffer = mUploadBuffer[GetUploadBufferIndex()];

		if (uploadBuffer == nullptr || mRDIResource == nullptr)
			return;

		_commandList->TranstionResourceState(mRDIResource, ERDIResourceState::Common, ERDIResourceState::CopyDest);

		if constexpr (std::is_same_v<RDIResourceType, IRDIBuffer>)
		{
			_commandList->CopyBuffer(mRDIResource, mUploadBuffer[GetUploadBufferIndex()]);
		}
		else
		{
			RDIResourceDescType resourceDesc;
			mRDIResource->GetDesc(&resourceDesc);

			ERDIPixelFormat pixelFormat = resourceDesc.mPixelFormat;
			uint32_t width = 1;
			uint32_t height = 1;
			uint32_t depth = 1;

			uint16_t mipCount = 1;
			uint16_t texArraySize = 1;

			if constexpr (CDescWithSizeX<RDIResourceDescType>)
				width = resourceDesc.mSizeX;
			if constexpr (CDescWithSizeY<RDIResourceDescType>)
				height = resourceDesc.mSizeY;

			if constexpr (CDescWithSizeXY<RDIResourceDescType>)
			{
				width = resourceDesc.mSizeXY;
				height = resourceDesc.mSizeXY;
			}

			if constexpr (CDescWithSizeZ<RDIResourceDescType>)
				depth = resourceDesc.mSizeZ;

			if constexpr (CDescWithArraySize<RDIResourceDescType>)
				texArraySize = resourceDesc.mArraySize;
			if constexpr (std::is_same_v<RDIResourceType, IRDITextureCube> || std::is_same_v<RDIResourceType, IRDITextureCubeArray>)
				texArraySize *= 6;

			if constexpr (CDescWithMipCount<RDIResourceDescType>)
				mipCount = resourceDesc.mMipCount;

			uint64_t bufferOffset = 0ull;

			for (uint16_t texIndex = 0; texIndex != texArraySize; ++texIndex)
			{
				for (uint16_t mipSlice = 0; mipSlice != mipCount; ++mipSlice)
				{
					if constexpr (std::is_same_v<RDIResourceType, IRDITexture1D>)
						_commandList->CopyTexture1D(mRDIResource, mipSlice, mUploadBuffer[GetUploadBufferIndex()], bufferOffset);
					else if constexpr (std::is_same_v<RDIResourceType, IRDITexture1DArray>)
						_commandList->CopyTexture1DArray(mRDIResource, mipSlice, texIndex, mUploadBuffer[GetUploadBufferIndex()], bufferOffset);
					else if constexpr (std::is_same_v<RDIResourceType, IRDITexture2D>)
						_commandList->CopyTexture2D(mRDIResource, mipSlice, mUploadBuffer[GetUploadBufferIndex()], bufferOffset);
					else if constexpr (std::is_same_v<RDIResourceType, IRDITexture2DArray>)
						_commandList->CopyTexture2DArray(mRDIResource, mipSlice, texIndex, mUploadBuffer[GetUploadBufferIndex()], bufferOffset);
					else if constexpr (std::is_same_v<RDIResourceType, IRDITexture3D>)
						_commandList->CopyTexture3D(mRDIResource, mipSlice, mUploadBuffer[GetUploadBufferIndex()], bufferOffset);
					else if constexpr (std::is_same_v<RDIResourceType, IRDITextureCube>)
						_commandList->CopyTextureCube(mRDIResource, mipSlice, static_cast<ERDITextureCubeFace>(texIndex), mUploadBuffer[GetUploadBufferIndex()], bufferOffset);
					else if constexpr (std::is_same_v<RDIResourceType, IRDITextureCubeArray>)
						_commandList->CopyTextureCubeArray(
							mRDIResource,
							static_cast<ERDITextureCubeFace>(texIndex % EnumToInt(ERDITextureCubeFace::FaceCount)),
							mipSlice,
							texIndex / EnumToInt(ERDITextureCubeFace::FaceCount),
							mUploadBuffer[GetUploadBufferIndex()],
							bufferOffset
						);

					bufferOffset += SPixelFormatMeta::GetPixelSlicePitch(pixelFormat, width, height, depth, mipSlice);
				}
			}
		}

		_commandList->TranstionResourceState(mRDIResource, ERDIResourceState::CopyDest, ERDIResourceState::Common);
	}

	void DestroyUploadBuffer() noexcept
	{
		for (size_t i = 0; i != GRenderInfoCount; ++i)
		{
			if (mUploadBuffer[i])
			{
				mUploadBuffer[i]->Release();
				mUploadBuffer[i] = nullptr;
			}
		}
	}

private:
	size_t GetUploadBufferIndex() const noexcept
	{
		return GetRenderModule()->GetFrameInfoIndex_RenderThread();
	}

	IRDIBuffer* CreateUploadBuffer()
	{
		SRDIBufferResourceDesc uploadBufferDesc;

		uploadBufferDesc.mHeapType = ERDIHeapType::Upload;
		uploadBufferDesc.mResourceUsage = ERDIResourceUsage::None;

		RDIResourceDescType resourceDesc;
		mRDIResource->GetDesc(&resourceDesc);

		if constexpr (std::is_same_v<RDIResourceType, IRDIBuffer>)
			uploadBufferDesc.mBufferSize = resourceDesc.mBufferSize;
		else if constexpr (std::is_same_v<RDIResourceType, IRDITexture1D>)
			uploadBufferDesc.mBufferSize = SPixelFormatMeta::GetPixelFootprint(resourceDesc.mPixelFormat, resourceDesc.mSizeX, 1, resourceDesc.mMipCount);
		else if constexpr (std::is_same_v<RDIResourceType, IRDITexture1DArray>)
			uploadBufferDesc.mBufferSize = SPixelFormatMeta::GetPixelFootprint(resourceDesc.mPixelFormat, resourceDesc.mSizeX, 1, resourceDesc.mMipCount, resourceDesc.mArraySize);
		else if constexpr (std::is_same_v<RDIResourceType, IRDITexture2D>)
			uploadBufferDesc.mBufferSize = SPixelFormatMeta::GetPixelFootprint(resourceDesc.mPixelFormat, resourceDesc.mSizeX, resourceDesc.mSizeY, resourceDesc.mMipCount);
		else if constexpr (std::is_same_v<RDIResourceType, IRDITexture2DArray>)
			uploadBufferDesc.mBufferSize = SPixelFormatMeta::GetPixelFootprint(resourceDesc.mPixelFormat, resourceDesc.mSizeX, resourceDesc.mSizeY, resourceDesc.mMipCount, resourceDesc.mArraySize);
		else if constexpr (std::is_same_v<RDIResourceType, IRDITexture3D>)
			uploadBufferDesc.mBufferSize = SPixelFormatMeta::GetPixelSlicePitch(resourceDesc.mPixelFormat, resourceDesc.mSizeX, resourceDesc.mSizeY, resourceDesc.mSizeZ, resourceDesc.mMipCount);
		else if constexpr (std::is_same_v<RDIResourceType, IRDITextureCube>)
			uploadBufferDesc.mBufferSize = SPixelFormatMeta::GetPixelFootprint(resourceDesc.mPixelFormat, resourceDesc.mSizeXY, resourceDesc.mSizeXY, resourceDesc.mMipCount, 6);
		else if constexpr (std::is_same_v<RDIResourceType, IRDITextureCubeArray>)
			uploadBufferDesc.mBufferSize = SPixelFormatMeta::GetPixelFootprint(resourceDesc.mPixelFormat, resourceDesc.mSizeXY, resourceDesc.mSizeXY, resourceDesc.mMipCount, resourceDesc.mArraySize * 6);

		return mRDIDevice->CreateBuffer(&uploadBufferDesc);
	}

private:
	IRDIDevice* mRDIDevice = nullptr;

	RDIResourceType* mRDIResource = nullptr;
	IRDIBuffer* mUploadBuffer[GRenderInfoCount] = {};
};

using RDynamicGPUBuffer = TDynamicGPUResrouce<IRDIBuffer>;
using RDynamicGPUTexture1D = TDynamicGPUResrouce<IRDITexture1D>;
using RDynamicGPUTexture1DArray = TDynamicGPUResrouce<IRDITexture1DArray>;
using RDynamicGPUTexture2D = TDynamicGPUResrouce<IRDITexture2D>;
using RDynamicGPUTexture2DArray = TDynamicGPUResrouce<IRDITexture2DArray>;
using RDynamicGPUTexture3D = TDynamicGPUResrouce<IRDITexture3D>;
using RDynamicGPUTextureCube = TDynamicGPUResrouce<IRDITextureCube>;
using RDynamicGPUTextureCubeArray = TDynamicGPUResrouce<IRDITextureCubeArray>;

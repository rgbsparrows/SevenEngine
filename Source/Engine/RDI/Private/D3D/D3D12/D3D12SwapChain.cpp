#include "D3D/D3DUtil.h"
#include "D3D12SwapChain.h"
#include "D3D/D3D12/D3D12Device.h"
#include "D3D/Warper/D3DImplWarper.h"
#include "D3D/D3D12/Warper/D3D12ImplWarper.h"
#include "D3D12Adapter.h"

void SD3D12SwapChain::Init(void* _nativePtr, const SRDISwapChainDesc* _desc, SD3D12Device* _device, SD3D12Adapter* _adapter) noexcept
{
	{
		mSwapChainNativePtr = _nativePtr;
		mDesc = *_desc;
		mDevice = _device;
		mAdapter = _adapter;
	}

	{
		SRDITexture2DResourceDesc desc;
		desc.mHeapType = ERDIHeapType::Default;
		desc.mResourceUsage = ERDIResourceUsage::RenderTarget;
		desc.mResourceState = ERDIResourceState::Common;
		desc.mPixelFormat = _desc->mPixelFormat;
		desc.mRtvPixelFormat = _desc->mPixelFormat;
		desc.mMipCount = 1;
		desc.mSizeX = _desc->mWidth;
		desc.mSizeY = _desc->mHeight;

		for (uint32_t i = 0; i != mDesc.mBufferCount; ++i)
		{
			void* renderTargetNativePtr = nullptr;
			VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12SwapChainGetBuffer_D3D12Impl(GetNativePtr(), i, &renderTargetNativePtr));
			mRenderTargets[i] = mDevice->CreateTexture2DWithCreatedResource(&desc, renderTargetNativePtr);
		}
	}

	{
		void* outputNativePtr = nullptr;
		D3DAPIWarp_Impl::GetContainingOutput_D3DImpl(GetNativePtr(), &outputNativePtr);

		for (auto _output : mAdapter->GetOutputs())
		{
			SD3D12Output* output = static_cast<SD3D12Output*>(_output);

			if (output->GetNativePtr() == outputNativePtr)
				mOutput = output;
		}
	}
}

void SD3D12SwapChain::Present() noexcept
{
	VERIFY_D3D_RETURN(D3DAPIWarp_Impl::Present_D3DImpl(GetNativePtr()));
}

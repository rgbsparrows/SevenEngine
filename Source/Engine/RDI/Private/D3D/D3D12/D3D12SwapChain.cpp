#include "D3D/D3DUtil.h"
#include "D3D12SwapChain.h"
#include "D3D/D3D12/D3D12Device.h"
#include "D3D12Adapter.h"

void SD3D12SwapChain::Init(IDXGISwapChain* _nativePtr, const SRDISwapChainDesc* _desc, SD3D12Device* _device, SD3D12Adapter* _adapter) noexcept
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
			ID3D12Resource* renderTargetNativePtr = nullptr;
			GetNativePtr()->GetBuffer(i, IID_PPV_ARGS(&renderTargetNativePtr));
			mRenderTargets[i] = mDevice->CreateTexture2DWithCreatedResource(&desc, renderTargetNativePtr);
		}
	}

	{
		IDXGIOutput* outputNativePtr = nullptr;
		GetNativePtr()->GetContainingOutput(&outputNativePtr);

		for (auto _output : mAdapter->GetOutputs())
		{
			SD3D12Output* output = static_cast<SD3D12Output*>(_output);

			if (output->GetNativePtr() == outputNativePtr)
				mOutput = output;
		}
		outputNativePtr->Release();
	}
}

void SD3D12SwapChain::Present() noexcept
{
	GetNativePtr()->Present(0, 0);
}

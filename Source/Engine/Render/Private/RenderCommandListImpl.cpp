#include "Core/Misc/Thread.h"
#include "Render/RenderContent.h"
#include "RenderCommandListImpl.h"
#include "RDI/Interface/RDISwapChain.h"
#include "RDI/Interface/RDICommandList.h"

bool SRenderCommandListImpl::HasImmediatelyRenderCommand() const noexcept
{
	return mCommandQueueBegin != mCommandQueueEnd;
}

void SRenderCommandListImpl::ExecuateImmediatelyRenderCommand(SRenderContent& _renderContent) noexcept
{
	while (HasImmediatelyRenderCommand())
	{
		mImmediatelyRenderCommandQueue[mCommandQueueBegin](_renderContent);
		mCommandQueueBegin = (mCommandQueueBegin + 1) % mImmediatelyRenderCommandQueue.size();
	}
}

void SRenderCommandListImpl::AddRenderCommand(std::function<void(SRenderContent&)> _renderCommand) noexcept
{
	if (mImmediatelyRenderCommandQueue.empty() || (mCommandQueueBegin == (mCommandQueueEnd + 1) % mImmediatelyRenderCommandQueue.size()))
	{
		RefrashImmediatelyRenderCommand();
		mImmediatelyRenderCommandQueue.resize(mImmediatelyRenderCommandQueue.size() + mImmediatelyRenderCommandQueue.size() / 2 + 10);
		mCommandQueueBegin = 0;
		mCommandQueueEnd = 0;
	}

	mImmediatelyRenderCommandQueue[mCommandQueueEnd] = std::move(_renderCommand);
	mCommandQueueEnd = (mCommandQueueEnd + 1) % mImmediatelyRenderCommandQueue.size();
}

void SRenderCommandListImpl::InitRenderProxy(RRenderProxyBase* _renderProxy) noexcept
{
	GetFrameResource_GameThread().mNeedInitRenderProxy.push_back(_renderProxy);
}

void SRenderCommandListImpl::InitRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept
{
	for (RRenderProxyBase* _renderProxy : _renderProxyList)
		InitRenderProxy(_renderProxy);
}

void SRenderCommandListImpl::AddExpiringRenderProxy(RRenderProxyBase* _renderProxy) noexcept
{
	GetFrameResource_GameThread().mExpiringRenderProxy.push_back(_renderProxy);
}

void SRenderCommandListImpl::AddExpiringRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept
{
	for (RRenderProxyBase* _renderProxy : _renderProxyList)
		AddExpiringRenderProxy(_renderProxy);
}

void SRenderCommandListImpl::RefrashImmediatelyRenderCommand() noexcept
{
	Thread::YieldUntilValue(mCommandQueueBegin, mCommandQueueEnd.load());
}

void SRenderCommandListImpl::RefrashStaticTexture2D_I(RRenderProxy<RTexture2D>* _texture2D, RTexture2DData&& _textureData) noexcept
{
	struct SRefrashStaticTexture2DCommand
	{
		RRenderProxy<RTexture2D>* mTexture2D = nullptr;
		RTexture2DData mTexture2DData;

		void operator()(SRenderContent& _renderContent) noexcept
		{
			RTexture2D& texture = mTexture2D->Get();

			bool needRecreate = false;

			if (texture.mTexture != nullptr)
			{
				SRDITexture2DResourceDesc desc;
				texture.mTexture->GetDesc(&desc);

				needRecreate = desc != mTexture2DData.mDesc;
			}

			if (needRecreate)
			{
				_renderContent.SyncToGpuFrameEnd();
				texture.mTexture->Release();
			}

			texture.mTexture = _renderContent.GetDevice()->CreateTexture2D(&mTexture2DData.mDesc);

			IRDIBuffer* uploadBuffer = nullptr;

			{
				SRDIBufferResourceDesc desc;
				desc.mHeapType = ERDIHeapType::Upload;
				desc.mResourceUsage = ERDIResourceUsage::None;
				desc.mBufferSize = SPixelFormatMeta::GetPixelFootprint(mTexture2DData.mDesc.mPixelFormat, mTexture2DData.mDesc.mSizeX, mTexture2DData.mDesc.mSizeY, mTexture2DData.mDesc.mMipCount);

				uploadBuffer = _renderContent.GetDevice()->CreateBuffer(&desc);

				void* dataPtr = nullptr;
				uploadBuffer->Map(&dataPtr);

#if WITH_DEBUG_CODE
				memset(dataPtr, 0, desc.mBufferSize);
#endif

				for (uint32_t i = 0; i != mTexture2DData.mDesc.mMipCount; ++i)
				{
					if (mTexture2DData.mSubresourceData[i].IsEmpty() == false)
						memcpy_s(dataPtr, desc.mBufferSize, mTexture2DData.mSubresourceData[i].GetBuffer(), mTexture2DData.mSubresourceData[i].GetBufferSize());

					dataPtr = reinterpret_cast<uint8_t*>(dataPtr) + SPixelFormatMeta::GetPixelSlicePitch(mTexture2DData.mDesc.mPixelFormat, mTexture2DData.mDesc.mSizeX, mTexture2DData.mDesc.mSizeY, i);
				}
				uploadBuffer->Unmap();
			}

			IRDICommandList* commandList = _renderContent.AllocateCommandList();
			for (uint32_t i = 0, offset = 0; i != mTexture2DData.mDesc.mMipCount; ++i)
			{
				commandList->CopyTexture2D(texture.mTexture, i, uploadBuffer, offset);
				offset += SPixelFormatMeta::GetPixelSlicePitch(mTexture2DData.mDesc.mPixelFormat, mTexture2DData.mDesc.mSizeX, mTexture2DData.mDesc.mSizeY, i);
			}
			commandList->TranstionResourceState(texture.mTexture, ERDIResourceState::CopyDest, ERDIResourceState::Common);
			commandList->Close();

			_renderContent.ExecuteCommandList(commandList);
			_renderContent.SyncToGpuFrameEnd(true);

			uploadBuffer->Release();
			commandList->Release();
		}
	};

	AddRenderCommand(SRefrashStaticTexture2DCommand{ _texture2D, std::move(_textureData)});
}

void SRenderCommandListImpl::RefrashImTexture2D_I(RRenderProxy<RTexture2D>* _texture2D, RRenderProxy<RImguiTexture2D>* _imTexture2D) noexcept
{
	struct SRefrashImTexture2D
	{
		RRenderProxy<RTexture2D>* mTexture2D;
		RRenderProxy<RImguiTexture2D>* mImTexture2D;

		void operator()(SRenderContent& _renderContent)
		{
			auto& texture2dInfo = mTexture2D->Get();
			auto& imTextureInfo = mImTexture2D->Get();

			imTextureInfo.mTexture2D = texture2dInfo.mTexture;
			if (imTextureInfo.mDescriptorHeapRange == nullptr)
				imTextureInfo.mDescriptorHeapRange = _renderContent.GetDevice()->CreateDescriptorRange(1, 0);
			imTextureInfo.mDescriptorHeapRange->SetSRV(0, imTextureInfo.mTexture2D->GetSRV());
		}
	};

	AddRenderCommand(SRefrashImTexture2D{_texture2D, _imTexture2D});
}

void SRenderCommandListImpl::RefrashSwapChain_I(RRenderProxy<RSwapChain>* _swapChain, const RSwapChainData& _swapChainData) noexcept
{
	struct SRefrashSwapChainCommand
	{
		RRenderProxy<RSwapChain>* mSwapChain;
		RSwapChainData mSwapChainData;

		void operator()(SRenderContent& _renderContent)
		{
			_renderContent.SyncToGpuFrameEnd();

			RSwapChain& swapChain = mSwapChain->Get();

			if (swapChain.mSwapChain != nullptr)
				swapChain.mSwapChain->Release();

			SRDISwapChainDesc swapChainDesc;
			swapChainDesc.mWidth = mSwapChainData.mWidth;
			swapChainDesc.mHeight = mSwapChainData.mHeight;
			swapChainDesc.mRefreshRate = mSwapChainData.mRefreshRate;
			swapChainDesc.mPixelFormat = mSwapChainData.mPixelFormat;
			swapChainDesc.mScalingMode = mSwapChainData.mScalingMode;
			swapChainDesc.mBufferCount = GRenderInfoCount;
			swapChainDesc.mOutputWindow = mSwapChainData.mOutputWindow;
			swapChainDesc.mIsWindowed = mSwapChainData.mIsWindowed;

			swapChain.mSwapChain = _renderContent.GetDevice()->CreateSwapChain(&swapChainDesc);
		}
	};

	AddRenderCommand(SRefrashSwapChainCommand{ _swapChain, _swapChainData });
}

void SRenderCommandListImpl::RenderWindow(RRenderProxy<RSwapChain>* _swapChain, RRenderProxy<RImguiDrawData>* _imguiDrawData) noexcept
{
	GetFrameResource_GameThread().mRenderWindowList.push_back(RRenderWindowInfo{ _swapChain, _imguiDrawData });
}

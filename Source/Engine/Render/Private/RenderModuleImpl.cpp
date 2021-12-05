#include "RDI/RDIModule.h"
#include "RenderResource.h"
#include "RenderModuleImpl.h"
#include "Core/Misc/windowsEx.h"
#include "RDI/Interface/RDIDevice.h"
#include "Core/Modules/ModuleManager.h"
#include "RDI/Interface/RDISwapChain.h"
#include "RDI/Interface/RDICommandList.h"
#include "RDI/Interface/RDICommandQueue.h"
#include "RDI/Interface/RDIDescriptorHeapRange.h"

#include <functional>

SRenderModuleImpl* GRenderModuleImpl = nullptr;

IRenderModule* GetRenderModule() noexcept
{
	return GRenderModuleImpl;
}

SRenderModuleImpl* GetRenderModuleImpl() noexcept
{
	return GRenderModuleImpl;
}

REGIST_MODULE(L"RenderModule", SRenderModuleImpl)

bool SRenderModuleImpl::Init() noexcept
{
	{
		GRenderModuleImpl = this;
	}

	{
		mFrameResource = new RRenderProxy<RFrameResource>;
		mStaticRenderResource = new RRenderProxy<RStaticRenderResource>;
		mFrameRenderResource = new RRenderProxy<RFrameRenderResource>;
	}

	{
		SModuleManager::Get().LoadModule(L"RDIModule");
		mRdiFactory = GetRDIModule()->GetRDIFactory();
		mRdiDevice = mRdiFactory->GetDevice();
		mRdiCommandQueue = mRdiDevice->GetCommandQueue();
	}

	{
		mFrameResource->Init(nullptr);
		mRenderCommandList.SetFrameResource(mFrameResource);
	}

	{
		mRenderThread = std::thread(std::mem_fn(&SRenderModuleImpl::RenderThreadMain), this);
	}

	{
		BeginFrame_GameThread();
		GetRenderCommandList()->InitRenderProxy({ mStaticRenderResource, mFrameRenderResource });
		EndFrame_GameThread();
	}

	return true;
}

void SRenderModuleImpl::Clear() noexcept
{
	{
		BeginFrame_GameThread();
		GetRenderCommandList()->AddExpiringRenderProxy({ mStaticRenderResource, mFrameRenderResource });
		EndFrame_GameThread();

		BeginFrame_GameThread();
		mFrameResource->Get_GameThread().mRequireExit = true;
		EndFrame_GameThread();

		mRenderThread.join();

		mFrameResource->Release();
		mFrameResource = nullptr;
		mStaticRenderResource = nullptr;
		mFrameRenderResource = nullptr;
	}

	{
		SModuleManager::Get().UnloadModule(L"RDIModule");
		GRenderModuleImpl = nullptr;
	}
}

void SRenderModuleImpl::BeginFrame_GameThread() noexcept
{
	YieldForSingleObject(mFrameResource->Get_GameThread().mGameThreadFrameResourceReadyEvent);
}

void SRenderModuleImpl::EndFrame_GameThread() noexcept
{
	SetEvent(mFrameResource->Get_GameThread().mRenderThreadFrameResourceReadyEvent);
	++mFrameInfoIndex_GameThread;
	mFrameInfoIndex_GameThread %= GRenderInfoCount;
}

void SRenderModuleImpl::BeginFrame_RenderThread() noexcept
{
	YieldForSingleObject(mFrameResource->Get_RenderThread().mRenderThreadFrameResourceReadyEvent);
	TODO("正确处理CommandAllocator")
	SyncToGpuFrameEnd(true);
	mRdiCommandQueue->YieldUntilCompletion(mFrameResource->Get_RenderThread().mGpuFence);
	mRdiDevice->ResetCommandListAlocator();
}

void SRenderModuleImpl::FrameTick_RenderThread() noexcept
{
	for (RRenderProxyBase* _renderProxy : mFrameResource->Get_RenderThread().mNeedInitRenderProxy)
		_renderProxy->Init(mRdiDevice);


	RefrashSwapChain();
	RefrashResources();

	TODO("世界绘制");

	RenderImgui();
	PresentWindows();

	for (RRenderProxyBase* _renderProxy : mFrameResource->Get_RenderThread().mExpiringRenderProxy)
		_renderProxy->Release();

	mFrameResource->Get_RenderThread().OnRenderFrameEnd();
}

void SRenderModuleImpl::EndFrame_RenderThread() noexcept
{
	mIsSyncToGpuFrameEnd = false;
	SetEvent(mFrameResource->Get_RenderThread().mGameThreadFrameResourceReadyEvent);
	mFrameResource->Get_RenderThread().mGpuFence = mRdiCommandQueue->Signal();
	++mFrameInfoIndex_RenderThread;
	mFrameInfoIndex_RenderThread %= GRenderInfoCount;
}

void SRenderModuleImpl::RenderThreadMain() noexcept
{
	while (mFrameResource->Get_RenderThread().mRequireExit == false)
	{
		BeginFrame_RenderThread();
		FrameTick_RenderThread();
		EndFrame_RenderThread();
	}
}

void SRenderModuleImpl::RefrashResources() noexcept
{
	RefrashTextureResource();
}

void SRenderModuleImpl::RefrashTextureResource() noexcept
{
	if (true)
	{
		auto& createStaticTexture2DList = mFrameResource->Get_RenderThread().mRefrashexture2DList;

		mRdiDevice->EnsureCommandListCount(1);

		IRDICommandList* commandList = mRdiDevice->GetCommandList(0);
		commandList->ResetCommandList();

		std::vector<IRDIBuffer*> uploadBufferList;

		for (auto& createStaticTexture2DInfo : createStaticTexture2DList)
		{
			RTexture2D& texture = createStaticTexture2DInfo.mTexture2D->Get_RenderThread();
			RTexture2DData& texture2DData = createStaticTexture2DInfo.mStaticTexture2DDataProxy == nullptr ? createStaticTexture2DInfo.mStaticTexture2DData : createStaticTexture2DInfo.mStaticTexture2DDataProxy->Get_RenderThread();

			bool needRecreate = false;

			if (texture.mTexture != nullptr)
			{
				SRDITexture2DResourceDesc desc;
				texture.mTexture->GetDesc(&desc);

				needRecreate = desc != texture2DData.mDesc;
			}

			if (needRecreate)
			{
				SyncToGpuFrameEnd();
				texture.mTexture->Release();
			}

			texture.mTexture = mRdiDevice->CreateTexture2D(&texture2DData.mDesc);

			IRDIBuffer* uploadBuffer = nullptr;

			{
				SRDIBufferResourceDesc desc;
				desc.mHeapType = ERDIHeapType::Upload;
				desc.mResourceUsage = ERDIResourceUsage::None;
				desc.mResourceState = ERDIResourceState::GenericRead;
				desc.mBufferSize = SPixelFormatMeta::GetPixelFootprint(texture2DData.mDesc.mPixelFormat, texture2DData.mDesc.mSizeX, texture2DData.mDesc.mSizeY, texture2DData.mDesc.mMipCount);

				uploadBuffer = mRdiDevice->CreateBuffer(&desc);
				uploadBufferList.push_back(uploadBuffer);

				void* dataPtr = nullptr;
				uploadBuffer->Map(&dataPtr);

#if WITH_DEBUG_CODE
				memset(dataPtr, 0, desc.mBufferSize);
#endif

				for (uint32_t i = 0; i != texture2DData.mDesc.mMipCount; ++i)
				{
					if (texture2DData.mSubresourceData[i].IsEmpty() == false)
						memcpy_s(dataPtr, desc.mBufferSize, texture2DData.mSubresourceData[i].GetBuffer(), texture2DData.mSubresourceData[i].GetBufferSize());
					
					dataPtr = reinterpret_cast<uint8_t*>(dataPtr) + SPixelFormatMeta::GetPixelSlicePitch(texture2DData.mDesc.mPixelFormat, texture2DData.mDesc.mSizeX, texture2DData.mDesc.mSizeY, i);
				}
				uploadBuffer->Unmap();
			}

			uint32_t offset = 0;
			for(uint32_t i = 0; i != texture2DData.mDesc.mMipCount; ++i)
			{
				commandList->CopyTexture2D(texture.mTexture, i, uploadBuffer, offset);
				offset += SPixelFormatMeta::GetPixelSlicePitch(texture2DData.mDesc.mPixelFormat, texture2DData.mDesc.mSizeX, texture2DData.mDesc.mSizeY, i);
			}
		}

		commandList->Close();
		mRdiCommandQueue->ExecuteCommandLists(1, &commandList);

		if (uploadBufferList.size() != 0)
		{
			SyncToGpuFrameEnd(true);

			for (IRDIBuffer* buffer : uploadBufferList)
			{
				buffer->Release();
			}
		}
	}

	if(false)
	{
		auto& createStaticTexture2DList = mFrameResource->Get_RenderThread().mRefrashexture2DList;

		mRdiDevice->EnsureCommandListCount(1);

		IRDICommandList* commandList = mRdiDevice->GetCommandList(0);
		commandList->ResetCommandList();

		std::vector<IRDITexture2D*> uploadTextureList;

		for (auto& createStaticTexture2DInfo : createStaticTexture2DList)
		{
			RTexture2D& texture = createStaticTexture2DInfo.mTexture2D->Get_RenderThread();
			RTexture2DData& texture2DData = createStaticTexture2DInfo.mStaticTexture2DDataProxy == nullptr ? createStaticTexture2DInfo.mStaticTexture2DData : createStaticTexture2DInfo.mStaticTexture2DDataProxy->Get_RenderThread();

			bool needRecreate = false;

			if (texture.mTexture != nullptr)
			{
				SRDITexture2DResourceDesc desc;
				texture.mTexture->GetDesc(&desc);

				needRecreate = desc != texture2DData.mDesc;
			}

			if (needRecreate)
			{
				SyncToGpuFrameEnd();
				texture.mTexture->Release();
				texture.mTexture = nullptr;
			}

			if (texture.mTexture == nullptr)
				texture.mTexture = mRdiDevice->CreateTexture2D(&texture2DData.mDesc);

			for (uint32_t i = 0; i != texture2DData.mSubresourceData.size(); ++i)
			{
				SBufferView subresourceData = texture2DData.mSubresourceData[i];
				texture.mTexture->WriteToSubResource(i, subresourceData.GetBuffer());
			}
		}

		commandList->Close();
		mRdiCommandQueue->ExecuteCommandLists(1, &commandList);
	}

	{
		auto& createImTexture2dList = mFrameResource->Get_RenderThread().mRefrashImTexture2DList;

		for (auto& createImTexture2DInfo : createImTexture2dList)
		{
			auto& imTextureInfo = createImTexture2DInfo.mImTexture->Get_RenderThread();
			auto& texture2dInfo = createImTexture2DInfo.mTexture2D->Get_RenderThread();

			imTextureInfo.mTexture2D = texture2dInfo.mTexture;
			imTextureInfo.mDescriptorHeapRange = mRdiDevice->CreateDescriptorRange(1, 0);
			imTextureInfo.mDescriptorHeapRange->SetSRV(0, imTextureInfo.mTexture2D->GetSRV());
		}
	}
}

void SRenderModuleImpl::RefrashSwapChain() noexcept
{
	auto& refrashSwapChainList = mFrameResource->Get_RenderThread().mRefrashSwapChainList;
	if (refrashSwapChainList.size() != 0)
	{
		SyncToGpuFrameEnd();
	}

	for (auto& _refrashSwapChainInfo : refrashSwapChainList)
	{
		RSwapChain& swapChain = _refrashSwapChainInfo.mSwapChain->Get_RenderThread();
		RSwapChainData& swapChainData = _refrashSwapChainInfo.mSwapChainData->Get_RenderThread();

		bool needRefrashSwapChain = true;

		if (swapChain.mSwapChain != nullptr)
			swapChain.mSwapChain->Release();

		SRDISwapChainDesc swapChainDesc;
		swapChainDesc.mWidth = swapChainData.mWidth;
		swapChainDesc.mHeight = swapChainData.mHeight;
		swapChainDesc.mRefreshRate = swapChainData.mRefreshRate;
		swapChainDesc.mPixelFormat = swapChainData.mPixelFormat;
		swapChainDesc.mScalingMode = swapChainData.mScalingMode;
		swapChainDesc.mBufferCount = GRenderInfoCount;
		swapChainDesc.mOutputWindow = swapChainData.mOutputWindow;
		swapChainDesc.mIsWindowed = swapChainData.mIsWindowed;

		swapChain.mSwapChain = mRdiDevice->CreateSwapChain(&swapChainDesc);
	}
}

void SRenderModuleImpl::RenderImgui() noexcept
{
	auto& renderWindowList = mFrameResource->Get_RenderThread().mRenderWindowList;
	auto& staticRenderResource = mStaticRenderResource->Get_RenderThread();
	auto& frameRenderResource = mFrameRenderResource->Get_RenderThread();

	if (renderWindowList.empty())
		return;

	mRdiDevice->EnsureCommandListCount(1);

	IRDICommandList* commandList = mRdiDevice->GetCommandList(0);
	commandList->ResetCommandList();

	{
		bool needRecreateBuffer = false;
		needRecreateBuffer = frameRenderResource.mConstantBuffer == nullptr;

		if (frameRenderResource.mConstantBuffer)
		{
			SRDIBufferResourceDesc desc;
			frameRenderResource.mConstantBuffer->GetDesc(&desc);
			needRecreateBuffer = needRecreateBuffer || desc.mBufferSize < renderWindowList.size() * 256;
		}

		if (needRecreateBuffer)
		{
			if (frameRenderResource.mConstantBuffer)
				frameRenderResource.mConstantBuffer->Release();

			SRDIBufferResourceDesc cbDesc;
			cbDesc.mHeapType = ERDIHeapType::Upload;
			cbDesc.mResourceState = ERDIResourceState::GenericRead;
			cbDesc.mResourceUsage = ERDIResourceUsage::ConstantBuffer;
			cbDesc.mBufferSize = 256 * renderWindowList.size() / 2 * 3;
			frameRenderResource.mConstantBuffer = mRdiDevice->CreateBuffer(&cbDesc);
		}

		void* cbData = nullptr;
		frameRenderResource.mConstantBuffer->Map(&cbData);
		for (size_t i = 0; i != renderWindowList.size(); ++i)
		{
			auto& drawData = renderWindowList[i].mImguiDrawData->Get_RenderThread();
			void* destMemory = static_cast<uint8_t*>(cbData) + 256 * i;

			float L = drawData.mDisplayPos[0];
			float R = drawData.mDisplayPos[0] + drawData.mDisplaySize[0];
			float T = drawData.mDisplayPos[1];
			float B = drawData.mDisplayPos[1] + drawData.mDisplaySize[1];

			Math::SFloat4x4 matrix = Math::SFloat4x4Raw
			{
				{ 2.0f / (R - L),		0.0f,				0.0f,       0.0f },
				{ 0.0f,					2.0f / (T - B),     0.0f,       0.0f },
				{ 0.0f,					0.0f,				0.5f,       0.0f },
				{ (R + L) / (L - R),	(T + B) / (B - T),  0.5f,       1.0f },
			};

			memcpy_s(destMemory, 256, matrix.GetData(), 256);
		}
		frameRenderResource.mConstantBuffer->Unmap();
	}

	for (size_t i = 0; i != renderWindowList.size(); ++i)
	{
		auto& drawData = renderWindowList[i].mImguiDrawData->Get_RenderThread();
		auto& swapChain = renderWindowList[i].mSwapChain->Get_RenderThread();

		bool needRecreateBuffer = false;
		needRecreateBuffer = drawData.mRDIVertexBuffer == nullptr;

		if (drawData.mRDIVertexBuffer)
		{
			SRDIBufferResourceDesc desc;
			drawData.mRDIVertexBuffer->GetDesc(&desc);
			needRecreateBuffer = needRecreateBuffer || desc.mBufferSize < drawData.mVertexBuffer.size() * sizeof(RImguiVertex);
		}

		if (needRecreateBuffer)
		{
			if (drawData.mRDIVertexBuffer != nullptr)
				drawData.mRDIVertexBuffer->Release();

			if (drawData.mRDIIndexBuffer != nullptr)
				drawData.mRDIIndexBuffer->Release();

			SRDIBufferResourceDesc vbDesc;
			vbDesc.mHeapType = ERDIHeapType::Upload;
			vbDesc.mResourceState = ERDIResourceState::GenericRead;
			vbDesc.mResourceUsage = ERDIResourceUsage::VertexBuffer;
			vbDesc.mBufferSize = sizeof(RImguiVertex) * drawData.mVertexBuffer.size() / 2 * 3;
			vbDesc.mElementStride = sizeof(RImguiVertex);
			drawData.mRDIVertexBuffer = mRdiDevice->CreateBuffer(&vbDesc);

			SRDIBufferResourceDesc ibDesc;
			ibDesc.mHeapType = ERDIHeapType::Upload;
			ibDesc.mResourceState = ERDIResourceState::GenericRead;
			ibDesc.mResourceUsage = ERDIResourceUsage::IndexBuffer;
			ibDesc.mBufferSize = sizeof(uint16_t) * drawData.mIndexBuffer.size() / 2 * 3;
			ibDesc.mElementStride = sizeof(uint16_t);
			drawData.mRDIIndexBuffer = mRdiDevice->CreateBuffer(&ibDesc);
		}

		void* vbData = nullptr;
		void* ibData = nullptr;
		drawData.mRDIVertexBuffer->Map(&vbData);
		drawData.mRDIIndexBuffer->Map(&ibData);

		memcpy_s(vbData, sizeof(RImguiVertex) * drawData.mVertexBuffer.size(), drawData.mVertexBuffer.data(), sizeof(RImguiVertex) * drawData.mVertexBuffer.size());
		memcpy_s(ibData, sizeof(uint16_t) * drawData.mIndexBuffer.size(), drawData.mIndexBuffer.data(), sizeof(uint16_t) * drawData.mIndexBuffer.size());

		drawData.mRDIVertexBuffer->Unmap();
		drawData.mRDIIndexBuffer->Unmap();

		SRDISwapChainDesc swapChainDesc;
		swapChain.mSwapChain->GetDesc(&swapChainDesc);
		switch (swapChainDesc.mPixelFormat)
		{
		case ERDIPixelFormat::R8G8B8A8_UNORM:
			commandList->SetGraphicsPipelineState(staticRenderResource.mImguiSDRPipelineState);
			break;
		case ERDIPixelFormat::R10G10B10A2_UNORM:
			commandList->SetGraphicsPipelineState(staticRenderResource.mImguiHDR10PipelineState);
			break;
		case ERDIPixelFormat::R16G16B16A16_FLOAT:
			commandList->SetGraphicsPipelineState(staticRenderResource.mImguiHDR1000PipelineState);
			break;
		default:
			CHECK(false && "不恰当的PipelineState");
		}

		commandList->SetGraphicsRootSignature(staticRenderResource.mImguiRootSignature);

		commandList->TranstionResourceState(swapChain.mSwapChain->GetRenderTarget(), ERDIResourceState::Present, ERDIResourceState::RenderTarget);

		IRDIRenderTargetView* renderTargetView[1] = { swapChain.mSwapChain->GetRenderTarget()->GetRTV(0) };
		commandList->OMSetRenderTargets(1, renderTargetView, nullptr);
		commandList->ClearRenderTargetView(swapChain.mSwapChain->GetRenderTarget()->GetRTV(0), Math::SFColor(0.f, 0.f, 0.f, 1.f));

		IRDIVertexBufferView* vbView = drawData.mRDIVertexBuffer->GetVBV();
		commandList->IASetVertexBuffer(0, 1, &vbView);
		commandList->IASetIndexBuffer(drawData.mRDIIndexBuffer->GetIBV());
		commandList->IASetPrimitiveTopology(ERDIPrimitiveTopology::TRIANGLELIST);

		commandList->SetGraphicsRootConstantBuffer(0, frameRenderResource.mConstantBuffer, i * 256);
		for (auto& _cmd : drawData.mCmdBuffer)
		{
			RImguiTexture2D& imguiTexture = _cmd.mTextureId->Get_RenderThread();
			commandList->SetGraphicsRootDescriptorTable(1, imguiTexture.mDescriptorHeapRange);
			commandList->RSSetScissorRects(1, &_cmd.mClipRect);
			commandList->DrawIndexedInstanced(_cmd.mVertexCount, 1, _cmd.mIndexOffset, _cmd.mVertexOffset);
		}

		commandList->TranstionResourceState(swapChain.mSwapChain->GetRenderTarget(), ERDIResourceState::RenderTarget, ERDIResourceState::Present);
	}

	commandList->Close();
	mRdiCommandQueue->ExecuteCommandLists(1, &commandList);
}

void SRenderModuleImpl::PresentWindows() noexcept
{
	auto& renderWindowList = mFrameResource->Get_RenderThread().mRenderWindowList;

	RFrameRenderResource& frameRenderResource = mFrameRenderResource->Get_RenderThread();

	for (auto& _renderWindow : renderWindowList)
	{
		RSwapChain& swapChain = _renderWindow.mSwapChain->Get_RenderThread();
		swapChain.mSwapChain->Present(frameRenderResource.mEnableVSync);
	}
}

void SRenderModuleImpl::SyncToGpuFrameEnd(bool _force) noexcept
{
	if (mIsSyncToGpuFrameEnd == false || _force == true)
	{
		uint64_t fence = mRdiCommandQueue->Signal();
		mRdiCommandQueue->YieldUntilCompletion(fence);
		mIsSyncToGpuFrameEnd = true;
	}
}

#include "RDI/RDIModule.h"
#include "Render/RenderProxy/Resource/RenderResource.h"
#include "RenderModuleImpl.h"
#include "Core/Misc/Thread.h"
#include "Core/Misc/windowsEx.h"
#include "RDI/Interface/RDIDevice.h"
#include "Core/Modules/ModuleManager.h"
#include "RDI/Interface/RDISwapChain.h"
#include "RDI/Interface/RDICommandList.h"
#include "RDI/Interface/RDICommandQueue.h"
#include "RDI/Interface/RDIDescriptorHeapRange.h"

#include <functional>
#include "RDI/Interface/RDICommandAllocator.h"

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
		mFrameResource->Init(mRdiDevice);
		mMainRenderContent.Init(mRdiDevice, mRdiCommandQueue);
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
		//两次循环清空渲染线程全部任务，保证GPU不再使用任何资源
		BeginFrame_GameThread();
		EndFrame_GameThread();

		BeginFrame_GameThread();
		EndFrame_GameThread();

		//清理全局对象
		BeginFrame_GameThread();
		GetRenderCommandList()->AddExpiringRenderProxy({ mStaticRenderResource, mFrameRenderResource });
		EndFrame_GameThread();

		//推出渲染线程
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
	Thread::YieldUntilValue(mFrameResource->Get_GameThread().mGameThreadFrameResourceReadyFlag, true);
	mFrameResource->Get_GameThread().mGameThreadFrameResourceReadyFlag = false;
}

void SRenderModuleImpl::EndFrame_GameThread() noexcept
{
	std::atomic_bool& renderThreadFrameResourceReadyFlag = mFrameResource->Get_GameThread().mRenderThreadFrameResourceReadyFlag;

	++mFrameCount_GameThread;
	mFrameInfoIndex_GameThread = mFrameCount_GameThread % GRenderInfoCount;

	renderThreadFrameResourceReadyFlag = true;
}

void SRenderModuleImpl::BeginFrame_RenderThread() noexcept
{
	mRdiCommandQueue->YieldUntilCompletion(mFrameResource->Get_RenderThread().mGpuFence);
	mMainRenderContent.BeginFrame();

	while (true)
	{
		Thread::YieldUntil(
			[&]() {return mRenderCommandList.HasImmediatelyRenderCommand() || mFrameResource->Get_RenderThread().mRenderThreadFrameResourceReadyFlag; }
		);

		if (mRenderCommandList.HasImmediatelyRenderCommand())
			mRenderCommandList.ExecuateImmediatelyRenderCommand(mMainRenderContent);

		if (mFrameResource->Get_RenderThread().mRenderThreadFrameResourceReadyFlag == true)
		{
			mFrameResource->Get_RenderThread().mRenderThreadFrameResourceReadyFlag = false;
			break;
		}
	}
}

void SRenderModuleImpl::FrameTick_RenderThread() noexcept
{
	for (RRenderProxyBase* _renderProxy : mFrameResource->Get_RenderThread().mNeedInitRenderProxy)
		_renderProxy->Init(mRdiDevice);

	TODO("RenderGraph");

	RenderImgui();
	PresentWindows();

	for (RRenderProxyBase* _renderProxy : mFrameResource->Get_RenderThread().mExpiringRenderProxy)
		_renderProxy->Release();

	mFrameResource->Get_RenderThread().OnRenderFrameEnd();
}

void SRenderModuleImpl::EndFrame_RenderThread() noexcept
{
	mMainRenderContent.EndFrame();

	std::atomic_bool& gameThreadFrameResourceReadyFlag = mFrameResource->Get_RenderThread().mGameThreadFrameResourceReadyFlag;

	mFrameResource->Get_RenderThread().mGpuFence = mRdiCommandQueue->Signal();
	++mFrameCount_RenderThread;
	mFrameInfoIndex_RenderThread = mFrameCount_RenderThread % GRenderInfoCount;

	gameThreadFrameResourceReadyFlag = true;
}

void SRenderModuleImpl::RenderThreadMain() noexcept
{
	Thread::SetCurrentThreadName(L"渲染线程");
	while (mFrameResource->Get_RenderThread().mRequireExit == false)
	{
		BeginFrame_RenderThread();
		FrameTick_RenderThread();
		EndFrame_RenderThread();
	}
}

void SRenderModuleImpl::RenderImgui() noexcept
{
	auto& renderWindowList = mFrameResource->Get_RenderThread().mRenderWindowList;
	auto& staticRenderResource = mStaticRenderResource->Get_RenderThread();
	auto& frameRenderResource = mFrameRenderResource->Get_RenderThread();

	if (renderWindowList.empty())
		return;

	IRDICommandList* commandList = mMainRenderContent.AllocateCommandList();

	{
		bool needRecreateUploadBuffer = frameRenderResource.mConstantUploadBuffer == nullptr;

		if (frameRenderResource.mConstantUploadBuffer)
		{
			SRDIBufferResourceDesc desc;
			frameRenderResource.mConstantUploadBuffer->GetDesc(&desc);
			needRecreateUploadBuffer = needRecreateUploadBuffer || desc.mBufferSize < renderWindowList.size() * 256;
		}
		if (needRecreateUploadBuffer)
		{
			if (frameRenderResource.mConstantUploadBuffer)
				frameRenderResource.mConstantUploadBuffer->Release();

			SRDIBufferResourceDesc cbDesc;
			cbDesc.mHeapType = ERDIHeapType::Upload;
			cbDesc.mResourceUsage = ERDIResourceUsage::None;
			cbDesc.mBufferSize = 256 * renderWindowList.size() * 2;
			frameRenderResource.mConstantUploadBuffer = mRdiDevice->CreateBuffer(&cbDesc);
		}

		void* cbData = nullptr;
		frameRenderResource.mConstantUploadBuffer->Map(&cbData);
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
		frameRenderResource.mConstantUploadBuffer->Unmap();
	}

	{
		bool needRecreateConstantBuffer = staticRenderResource.mConstantBuffer == nullptr;

		if (staticRenderResource.mConstantBuffer)
		{
			SRDIBufferResourceDesc desc;
			staticRenderResource.mConstantBuffer->GetDesc(&desc);
			needRecreateConstantBuffer = needRecreateConstantBuffer || desc.mBufferSize < renderWindowList.size() * 256;
		}
		if (needRecreateConstantBuffer)
		{
			mMainRenderContent.SyncToGpuFrameEnd();

			if (staticRenderResource.mConstantBuffer)
				staticRenderResource.mConstantBuffer->Release();

			SRDIBufferResourceDesc cbDesc;
			cbDesc.mHeapType = ERDIHeapType::Default;
			cbDesc.mResourceUsage = ERDIResourceUsage::ConstantBuffer;
			cbDesc.mBufferSize = 256 * renderWindowList.size() * 2;
			staticRenderResource.mConstantBuffer = mRdiDevice->CreateBuffer(&cbDesc);
		}

		commandList->TranstionResourceState(staticRenderResource.mConstantBuffer, ERDIResourceState::Common, ERDIResourceState::CopyDest);
		commandList->CopyBuffer(staticRenderResource.mConstantBuffer, frameRenderResource.mConstantUploadBuffer);
		commandList->TranstionResourceState(staticRenderResource.mConstantBuffer, ERDIResourceState::CopyDest, ERDIResourceState::VertexAndConstantBuffer);
	}

	for (size_t i = 0; i != renderWindowList.size(); ++i)
	{
		auto& drawData = renderWindowList[i].mImguiDrawData->Get_RenderThread();
		auto& swapChain = renderWindowList[i].mSwapChain->Get_RenderThread();

		bool needRecreateVertexBuffer = drawData.mRDIVertexBuffer == nullptr;
		bool needRecreateIndexBuffer = drawData.mRDIIndexBuffer == nullptr;

		if (drawData.mRDIVertexBuffer)
		{
			SRDIBufferResourceDesc desc;
			drawData.mRDIVertexBuffer->GetDesc(&desc);
			needRecreateVertexBuffer = needRecreateVertexBuffer || desc.mBufferSize < drawData.mVertexBuffer.size() * sizeof(RImguiVertex);
		}

		if (drawData.mRDIIndexBuffer)
		{
			SRDIBufferResourceDesc desc;
			drawData.mRDIIndexBuffer->GetDesc(&desc);
			needRecreateIndexBuffer = needRecreateIndexBuffer || desc.mBufferSize < drawData.mIndexBuffer.size() * sizeof(uint16_t);
		}

		if (needRecreateVertexBuffer)
		{
			if (drawData.mRDIVertexBuffer != nullptr)
				drawData.mRDIVertexBuffer->Release();

			SRDIBufferResourceDesc vbDesc;
			vbDesc.mHeapType = ERDIHeapType::Upload;
			vbDesc.mResourceUsage = ERDIResourceUsage::VertexBuffer;
			vbDesc.mBufferSize = sizeof(RImguiVertex) * drawData.mVertexBuffer.size() * 2;
			vbDesc.mElementStride = sizeof(RImguiVertex);
			drawData.mRDIVertexBuffer = mRdiDevice->CreateBuffer(&vbDesc);
		}

		if (needRecreateIndexBuffer)
		{
			if (drawData.mRDIIndexBuffer != nullptr)
				drawData.mRDIIndexBuffer->Release();

			SRDIBufferResourceDesc ibDesc;
			ibDesc.mHeapType = ERDIHeapType::Upload;
			ibDesc.mResourceUsage = ERDIResourceUsage::IndexBuffer;
			ibDesc.mBufferSize = sizeof(uint16_t) * drawData.mIndexBuffer.size() * 2;
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

		commandList->TranstionResourceState(swapChain.mSwapChain->GetRenderTarget(), ERDIResourceState::Common, ERDIResourceState::RenderTarget);

		IRDIRenderTargetView* renderTargetView[1] = { swapChain.mSwapChain->GetRenderTarget()->GetRTV(0) };
		commandList->OMSetRenderTargets(1, renderTargetView, nullptr);
		commandList->ClearRenderTargetView(swapChain.mSwapChain->GetRenderTarget()->GetRTV(0), Math::SFColor(0.f, 0.f, 0.f, 1.f));

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

		IRDIVertexBufferView* vbView = drawData.mRDIVertexBuffer->GetVBV();
		commandList->IASetVertexBuffer(0, 1, &vbView);
		commandList->IASetIndexBuffer(drawData.mRDIIndexBuffer->GetIBV());
		commandList->IASetPrimitiveTopology(ERDIPrimitiveTopology::TRIANGLELIST);

		Math::SFloatBox viewportBox = Math::SFloatBox(0, 0, 0, drawData.mDisplaySize[0], drawData.mDisplaySize[1], 1);
		commandList->RSSetViewports(1, &viewportBox);

		commandList->SetGraphicsRootConstantBuffer(0, staticRenderResource.mConstantBuffer, i * 256);
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
	commandList->Release();
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

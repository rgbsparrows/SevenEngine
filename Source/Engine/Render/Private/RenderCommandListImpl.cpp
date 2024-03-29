﻿#include "Core/Misc/Thread.h"
#include "Render/RenderContext.h"
#include "RenderCommandListImpl.h"
#include "RDI/Interface/RDIDevice.h"
#include "RDI/Interface/RDISwapChain.h"
#include "RDI/Interface/RDICommandList.h"
#include "Render/RenderProxy/RenderProxy.h"
#include "Core/ProgramConfiguation/BuildConfiguation.h"

bool SRenderCommandListImpl::HasImmediatelyRenderCommand() const noexcept
{
	return mCommandQueueBegin != mCommandQueueEnd;
}

void SRenderCommandListImpl::ExecuateImmediatelyRenderCommand(SRenderContext& _renderContext) noexcept
{
	while (HasImmediatelyRenderCommand())
	{
		mImmediatelyRenderCommandQueue[mCommandQueueBegin](_renderContext);
		mCommandQueueBegin = (mCommandQueueBegin + 1) % mImmediatelyRenderCommandQueue.size();
	}
}

void SRenderCommandListImpl::AddRenderCommand(std::function<void(SRenderContext&)> _renderCommand) noexcept
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

void SRenderCommandListImpl::AddInitRenderProxy(RRenderProxyBase* _renderProxy) noexcept
{
	GetDeferTaskList_GameThread().mNeedInitRenderProxy.push_back(_renderProxy);
}

void SRenderCommandListImpl::AddInitRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept
{
	for (RRenderProxyBase* _renderProxy : _renderProxyList)
		AddInitRenderProxy(_renderProxy);
}

void SRenderCommandListImpl::AddExpiringRenderProxy(RRenderProxyBase* _renderProxy) noexcept
{
	GetDeferTaskList_GameThread().mExpiringRenderProxy.push_back(_renderProxy);
}

void SRenderCommandListImpl::AddExpiringRenderProxy(std::initializer_list<RRenderProxyBase*> _renderProxyList) noexcept
{
	for (RRenderProxyBase* _renderProxy : _renderProxyList)
		AddExpiringRenderProxy(_renderProxy);
}

void SRenderCommandListImpl::AddInitRenderGraph(RRenderGraphBase* _renderGraph) noexcept
{
	if (_renderGraph)
		GetDeferTaskList_GameThread().mNeedInitRenderGraph.push_back(_renderGraph);
}

void SRenderCommandListImpl::AddExpiringRenderGraph(RRenderGraphBase* _renderGraph) noexcept
{
	if (_renderGraph)
		GetDeferTaskList_GameThread().mExpiringRenderGraph.push_back(_renderGraph);
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

		void operator()(SRenderContext& _renderContext) noexcept
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
				_renderContext.SyncToGpuFrameEnd();
				texture.mTexture->Release();
			}

			texture.mTexture = _renderContext.GetDevice()->CreateTexture2D(&mTexture2DData.mDesc);

			if (mTexture2DData.mSubresourceData.empty() == false)
			{
				IRDIBuffer* uploadGPUBuffer = nullptr;
				{
					SRDIBufferResourceDesc desc;
					desc.mHeapType = ERDIHeapType::Upload;
					desc.mResourceUsage = ERDIResourceUsage::None;
					desc.mBufferSize = SPixelFormatMeta::GetPixelFootprint(mTexture2DData.mDesc.mPixelFormat, mTexture2DData.mDesc.mSizeX, mTexture2DData.mDesc.mSizeY, mTexture2DData.mDesc.mMipCount);

					uploadGPUBuffer = _renderContext.GetDevice()->CreateBuffer(&desc);

					SBufferView uploadBufferView = uploadGPUBuffer->Map();

					if constexpr (SBuildConfiguation::GIsDebugMode)
						memset(uploadBufferView.GetBuffer(), 0, desc.mBufferSize);

					size_t offset = 0;
					for (uint32_t i = 0; i != mTexture2DData.mDesc.mMipCount; ++i)
					{
						size_t pitchSize = SPixelFormatMeta::GetPixelSlicePitch(mTexture2DData.mDesc.mPixelFormat, mTexture2DData.mDesc.mSizeX, mTexture2DData.mDesc.mSizeY, i);

						if (mTexture2DData.mSubresourceData[i].IsEmpty() == false)
							Memcpy(SRange(offset, pitchSize).GetBuffer(uploadBufferView), mTexture2DData.mSubresourceData[i].GetBuffer(mTexture2DData.mResourceData));

						offset += pitchSize;
					}
					uploadGPUBuffer->Unmap();
				}

				_renderContext.AddRenderTask(u8"更新贴图", [&](IRDICommandList* _commandList)
					{
						for (uint32_t i = 0, offset = 0; i != mTexture2DData.mDesc.mMipCount; ++i)
						{
							_commandList->CopyTexture2D(texture.mTexture, i, uploadGPUBuffer, offset);
							offset += SPixelFormatMeta::GetPixelSlicePitch(mTexture2DData.mDesc.mPixelFormat, mTexture2DData.mDesc.mSizeX, mTexture2DData.mDesc.mSizeY, i);
						}
						_commandList->TranstionResourceState(texture.mTexture, ERDIResourceState::CopyDest, ERDIResourceState::Common);
					});

				_renderContext.ExecuteRenderGraph();
				_renderContext.SyncToGpuFrameEnd(true);

				uploadGPUBuffer->Release();
			}
		}
	};

	AddRenderCommand(SRefrashStaticTexture2DCommand{ _texture2D, std::move(_textureData) });
}

void SRenderCommandListImpl::RefrashImTexture2D_I(RRenderProxy<RTexture2D>* _texture2D, RRenderProxy<RImguiTexture2D>* _imTexture2D) noexcept
{
	struct SRefrashImTexture2D
	{
		RRenderProxy<RTexture2D>* mTexture2D;
		RRenderProxy<RImguiTexture2D>* mImTexture2D;

		void operator()(SRenderContext& _renderContext) noexcept
		{
			auto& texture2dInfo = mTexture2D->Get();
			auto& imTextureInfo = mImTexture2D->Get();

			imTextureInfo.mTexture2D = texture2dInfo.mTexture;
			if (imTextureInfo.mDescriptorHeapRange == nullptr)
				imTextureInfo.mDescriptorHeapRange = _renderContext.GetDevice()->CreateDescriptorRange(1, 0);
			imTextureInfo.mDescriptorHeapRange->SetSRV(0, imTextureInfo.mTexture2D->GetSRV());
		}
	};

	AddRenderCommand(SRefrashImTexture2D{ _texture2D, _imTexture2D });
}

void SRenderCommandListImpl::RefrashSwapChain_I(RRenderProxy<RSwapChain>* _swapChain, const RSwapChainData& _swapChainData) noexcept
{
	struct SRefrashSwapChainCommand
	{
		RRenderProxy<RSwapChain>* mSwapChain;
		RSwapChainData mSwapChainData;

		void operator()(SRenderContext& _renderContext) noexcept
		{
			_renderContext.SyncToGpuFrameEnd();

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

			swapChain.mSwapChain = _renderContext.GetDevice()->CreateSwapChain(&swapChainDesc);
		}
	};

	AddRenderCommand(SRefrashSwapChainCommand{ _swapChain, _swapChainData });
}

void SRenderCommandListImpl::RefrashMesh_I(RRenderProxy<RMesh>* _mesh, RMeshData _meshData) noexcept
{
	struct SRefrashMeshCommand
	{
		RRenderProxy<RMesh>* mMeshProxy;
		RMeshData mMeshData;

		IRDIBuffer* CreateIndexBuffer(SRenderContext& _renderContext, const uint32_t* _indexData, size_t _indexCount, IRDIBuffer** _uploadBuffer) noexcept
		{
			IRDIDevice* device = _renderContext.GetDevice();

			SRDIBufferResourceDesc indexBufferDesc;
			indexBufferDesc.mResourceUsage = ERDIResourceUsage::IndexBuffer;
			indexBufferDesc.mBufferSize = _indexCount * sizeof(uint32_t);
			indexBufferDesc.mElementStride = sizeof(uint32_t);

			IRDIBuffer* indexBuffer = device->CreateBuffer(&indexBufferDesc);

			SRDIBufferResourceDesc uploadBufferDesc;
			uploadBufferDesc.mHeapType = ERDIHeapType::Upload;
			uploadBufferDesc.mResourceUsage = ERDIResourceUsage::None;
			uploadBufferDesc.mBufferSize = _indexCount * sizeof(uint32_t);

			*_uploadBuffer = device->CreateBuffer(&uploadBufferDesc);

			SBufferView uploadBufferView = (*_uploadBuffer)->Map();
			Memcpy(uploadBufferView, _indexData, _indexCount * sizeof(uint32_t));
			(*_uploadBuffer)->Unmap();

			return indexBuffer;
		}

		IRDIBuffer* CreateVertexBuffer(SRenderContext& _renderContext, const void* _vertexData, size_t _vertexCount, uint32_t _vertexStride, IRDIBuffer** _uploadBuffer) noexcept
		{
			IRDIDevice* device = _renderContext.GetDevice();

			SRDIBufferResourceDesc vertexBufferDesc;
			vertexBufferDesc.mResourceUsage = ERDIResourceUsage::VertexBuffer;
			vertexBufferDesc.mBufferSize = _vertexCount * _vertexStride;
			vertexBufferDesc.mElementStride = _vertexStride;

			IRDIBuffer* vertexBuffer = device->CreateBuffer(&vertexBufferDesc);

			SRDIBufferResourceDesc uploadBufferDesc;
			uploadBufferDesc.mHeapType = ERDIHeapType::Upload;
			uploadBufferDesc.mResourceUsage = ERDIResourceUsage::None;
			uploadBufferDesc.mBufferSize = _vertexCount * _vertexStride;

			*_uploadBuffer = device->CreateBuffer(&uploadBufferDesc);
			 
			void* dataPtr = nullptr;
			SBufferView uploadBufferView = (*_uploadBuffer)->Map();
			Memcpy(uploadBufferView, _vertexData, _vertexCount * _vertexStride);
			(*_uploadBuffer)->Unmap();

			return vertexBuffer;
		}

		void operator()(SRenderContext& _renderContext) noexcept
		{
			const EVertexSemantic vertexSemantic[] = {
					EVertexSemantic::Position,
					EVertexSemantic::Color,
					EVertexSemantic::Normal,
					EVertexSemantic::Tangent,
					EVertexSemantic::BlendIndices,
					EVertexSemantic::BlendWeight,
					EVertexSemantic::Uv0,
					EVertexSemantic::Uv1,
					EVertexSemantic::Uv2,
					EVertexSemantic::Uv3,
					EVertexSemantic::Uv4,
					EVertexSemantic::Uv5,
					EVertexSemantic::Uv6,
					EVertexSemantic::Uv7,
			};

			const void* vertexData[] = {
				mMeshData.mPositionBuffer.data(),
				mMeshData.mColorBuffer.data(),
				mMeshData.mNormalBuffer.data(),
				mMeshData.mTangentBuffer.data(),
				mMeshData.mBlendIndicesBuffer.data(),
				mMeshData.mBlendWeightBuffer.data(),
				mMeshData.mUvBuffer[0].data(),
				mMeshData.mUvBuffer[1].data(),
				mMeshData.mUvBuffer[2].data(),
				mMeshData.mUvBuffer[3].data(),
				mMeshData.mUvBuffer[4].data(),
				mMeshData.mUvBuffer[5].data(),
				mMeshData.mUvBuffer[6].data(),
				mMeshData.mUvBuffer[7].data(),
			};

			const size_t vertexCount[] = {
				mMeshData.mPositionBuffer.size(),
				mMeshData.mColorBuffer.size(),
				mMeshData.mNormalBuffer.size(),
				mMeshData.mTangentBuffer.size(),
				mMeshData.mBlendIndicesBuffer.size(),
				mMeshData.mBlendWeightBuffer.size(),
				mMeshData.mUvBuffer[0].size(),
				mMeshData.mUvBuffer[1].size(),
				mMeshData.mUvBuffer[2].size(),
				mMeshData.mUvBuffer[3].size(),
				mMeshData.mUvBuffer[4].size(),
				mMeshData.mUvBuffer[5].size(),
				mMeshData.mUvBuffer[6].size(),
				mMeshData.mUvBuffer[7].size(),
			};

			const uint32_t vertexStride[] = {
				sizeof(Math::SFloat3),
				sizeof(Math::SFloat4),
				sizeof(Math::SFloat3),
				sizeof(Math::SFloat4),
				sizeof(Math::SUShort4),
				sizeof(Math::SFloat4),
				sizeof(Math::SFloat2),
				sizeof(Math::SFloat2),
				sizeof(Math::SFloat2),
				sizeof(Math::SFloat2),
				sizeof(Math::SFloat2),
				sizeof(Math::SFloat2),
				sizeof(Math::SFloat2),
				sizeof(Math::SFloat2),
			};

			RMesh& mesh = mMeshProxy->Get_RenderThread();

			IRDIBuffer* uploadBuffer[EnumToInt(EVertexSemantic::Num) + 1] = {};

			if (mesh.mIndexBuffer)
				mesh.mIndexBuffer->Release();

			for (size_t i = 0; i != ArraySize(vertexData); ++i)
			{
				if (mesh.mVertexBuffer[i])
					mesh.mVertexBuffer[i]->Release();
			}

			mesh.mVertexSemantic = mMeshData.mVertexSemantic;

			if (mMeshData.mIndexBuffer.size() != 0)
				mesh.mIndexBuffer = CreateIndexBuffer(_renderContext, mMeshData.mIndexBuffer.data(), mMeshData.mIndexBuffer.size(), &uploadBuffer[0]);

			for (size_t i = 0; i != ArraySize(vertexData); ++i)
			{
				if (vertexData[i])
					mesh.mVertexBuffer[EnumToInt(vertexSemantic[i])] = CreateVertexBuffer(_renderContext, vertexData[i], vertexCount[i], vertexStride[i], &uploadBuffer[i + 1]);
			}

			mesh.mSubMeshRange = mMeshData.mSubMeshRange;
			mesh.mVertexSemantic = mMeshData.mVertexSemantic;

			_renderContext.AddRenderTask("更新顶点数据", [&](IRDICommandList* _commandList) 
				{
					if (mMeshData.mIndexBuffer.size() != 0)
					{
						_commandList->TranstionResourceState(mesh.mIndexBuffer, ERDIResourceState::Common, ERDIResourceState::CopyDest);
						_commandList->CopyBuffer(mesh.mIndexBuffer, uploadBuffer[0]);
						_commandList->TranstionResourceState(mesh.mIndexBuffer, ERDIResourceState::CopyDest, ERDIResourceState::Common);
					}

					for (size_t i = 0; i != ArraySize(vertexData); ++i)
					{
						if (vertexData[i])
						{
							_commandList->TranstionResourceState(mesh.mVertexBuffer[i], ERDIResourceState::Common, ERDIResourceState::CopyDest);
							_commandList->CopyBuffer(mesh.mVertexBuffer[i], uploadBuffer[i + 1]);
							_commandList->TranstionResourceState(mesh.mVertexBuffer[i], ERDIResourceState::CopyDest, ERDIResourceState::Common);
						}
					}
				});

			_renderContext.ExecuteRenderGraph();
			_renderContext.SyncToGpuFrameEnd(true);

			for (size_t i = 0; i != ArraySize(uploadBuffer); ++i)
			{
				if (uploadBuffer[i])
					uploadBuffer[i]->Release();
			}
		}
	};

	AddRenderCommand(SRefrashMeshCommand{ _mesh, _meshData });
}

void SRenderCommandListImpl::RenderWorld_D(RRenderProxy<RWorld>* _world, const RCamera& _camera, RRenderProxy<RTexture2D>* _canvas, RRenderGraphBase* _renderGraph) noexcept
{
	GetDeferTaskList_GameThread().mRenderWorldList.push_back(RRenderWorldInfo{ _world, _camera, _canvas, _renderGraph});
}

void SRenderCommandListImpl::RenderWindow_D(RRenderProxy<RSwapChain>* _swapChain, RRenderProxy<RImguiDrawData>* _imguiDrawData, bool enableVSync) noexcept
{
	GetDeferTaskList_GameThread().mRenderWindowList.push_back(RRenderWindowInfo{ _swapChain, _imguiDrawData, enableVSync });
}

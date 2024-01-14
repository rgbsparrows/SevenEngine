#include "RDI/RDIModule.h"
#include "Render/RenderContext.h"
#include "Renderer/BaseRenderer.h"
#include "RDI/Interface/RDIDevice.h"
#include "RDI/Interface/RDIFactory.h"
#include "RDI/Interface/RDICommandList.h"
#include "Render/RenderProxy/World/WorldProxy.h"
#include "Render/RenderProxy/Resource/RenderResource.h"

void RBaseRenderer::Init(SRenderContext& _renderContext) noexcept
{
	SBlob ShaderCodeBuffer;
	{
		FILE* fp;
		fopen_s(&fp, "D:/Project/SevenEngine/Shaders/simple.shader", "rb");
		fseek(fp, 0, SEEK_END);
		long len = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		ShaderCodeBuffer.ResizeBlob(len);
		fread(ShaderCodeBuffer.GetBuffer(), 1, len, fp);
		fclose(fp);
	}

	SRDIGraphicsPipelineStateDesc pipelineStateDesc;

	pipelineStateDesc.mRootSignature.mRootParameters.resize(2);

	pipelineStateDesc.mRootSignature.mRootParameters[0].mType = ERDIRootParameterType::CBV;
	pipelineStateDesc.mRootSignature.mRootParameters[0].mRegisterSpace = 0;
	pipelineStateDesc.mRootSignature.mRootParameters[0].mCbvRootParameter.mCbvShaderRegister = 0;
	
	pipelineStateDesc.mRootSignature.mRootParameters[1].mType = ERDIRootParameterType::CBV;
	pipelineStateDesc.mRootSignature.mRootParameters[1].mRegisterSpace = 0;
	pipelineStateDesc.mRootSignature.mRootParameters[1].mCbvRootParameter.mCbvShaderRegister = 1;

	SRDIErrorInfo vserrror;
	SRDIErrorInfo pserrror;

	SBlob vsBlob = GetRDIModule()->GetRDIFactory()->CompileVertexShader(ShaderCodeBuffer);
	SBlob psBlob = GetRDIModule()->GetRDIFactory()->CompilePixelShader(ShaderCodeBuffer);
	pipelineStateDesc.mVertexShader = vsBlob;
	pipelineStateDesc.mPixelShader = psBlob;

	SRDIVertexInputElememt VertexInputElement;
	VertexInputElement.mSemanticName = "POSITION";
	VertexInputElement.mFormat = ERDIPixelFormat::R32G32B32_FLOAT;
	VertexInputElement.mAlignedByteOffset = 0;

	pipelineStateDesc.mInputLayout.mInputElements.push_back(VertexInputElement);

	pipelineStateDesc.mRasterizationState.mCullMode = ERDICullMode::None;
	pipelineStateDesc.mPrimitiveTopologyType = ERDIPrimitiveTopologyType::TRIANGLE;
	pipelineStateDesc.mRenderTargetFormat[0] = ERDIPixelFormat::R8G8B8A8_UNORM;
	pipelineStateDesc.mDepthStencilFormat = ERDIPixelFormat::UNKNOWN;

	mPipelineState = _renderContext.GetDevice()->CreateGraphicsPipelineState(&pipelineStateDesc);
}

void RBaseRenderer::Render(RWorld& _renderData, RCamera& _camera, RTexture2D& _canvas, SRenderContext& _renderContext) noexcept
{
	SRDITexture2DResourceDesc canvasTextureDesc;
	_canvas.mTexture->GetDesc(&canvasTextureDesc);

	Math::SFloat4x4 projMatrix = Math::CalcPerspectiveProjectMatrix(_camera.mFov, _camera.mNearClip, _camera.mFarClip, canvasTextureDesc.mSizeX, canvasTextureDesc.mSizeY);
	Math::SFloat4x4 viewMatrix = Math::CalcTransformViewMatrix(_camera.mPosition, _camera.mRotation);

	viewMatrix = Math::TransposeMatrix(viewMatrix);
	projMatrix = Math::TransposeMatrix(projMatrix);

	if (mDynamicConstantBuffer.GetDesc().mBufferSize < _renderData.mStaticMeshList.size() * 10 * 256 + 256)
	{
		_renderContext.SyncToGpuFrameEnd();

		SRDIBufferResourceDesc desc;
		desc.mResourceUsage = ERDIResourceUsage::ConstantBuffer;
		desc.mBufferSize = _renderData.mStaticMeshList.size() * 10 * 256 + 256;

		mDynamicConstantBuffer = RDynamicGPUBuffer::ConstructDynamicGPUResource(_renderContext.GetDevice(), &desc);
	}

	SBufferView constantBuffer = mDynamicConstantBuffer.Map();

	Memcpy(SRange(0, sizeof(Math::SFloat4x4)).GetBuffer(constantBuffer), viewMatrix.GetData(), sizeof(Math::SFloat4x4));
	Memcpy(SRange(sizeof(Math::SFloat4x4), 2 * sizeof(Math::SFloat4x4)).GetBuffer(constantBuffer), projMatrix.GetData(), sizeof(Math::SFloat4x4));

	size_t dcIndex = 0;
	for (size_t i = 0; i != _renderData.mStaticMeshList.size(); ++i)
	{
		RStaticMeshProxy& meshProxy = _renderData.mStaticMeshList[i];

		RMesh& Mesh = meshProxy.mMesh->Get_RenderThread();

		for (size_t j = 0; j != meshProxy.mMeshColor.size(); ++j)
		{
			SBufferView info1BufferView = SRange(dcIndex * 256 + 256, sizeof(Math::SFColor) + sizeof(Math::SFloat4x4)).GetBuffer(constantBuffer);

			Memcpy(SRange(0, sizeof(Math::SFColor)).GetBuffer(info1BufferView), &meshProxy.mMeshColor[j], sizeof(Math::SFColor));
			Memcpy(SRange(sizeof(Math::SFColor), sizeof(Math::SFloat4x4)).GetBuffer(info1BufferView), Math::TransposeMatrix(meshProxy.mWorldMatrix).GetData(), sizeof(Math::SFloat4x4));

			++dcIndex;
		}
	}

	mDynamicConstantBuffer.Unmap();

	SRenderTaskIdentify updateCbTask = _renderContext.AddRenderTask(u8"CB 更新", [=](IRDICommandList* _commandList)
		{
			mDynamicConstantBuffer.Upload(_commandList);
		}
	);

	_renderContext.AddRenderTask(u8"前向绘制", [=, &_renderData](IRDICommandList* _commandList)
		{
			_commandList->TranstionResourceState(_canvas.mTexture, ERDIResourceState::Common, ERDIResourceState::RenderTarget);
			_commandList->ClearRenderTargetView(_canvas.mTexture->GetRTV(0));

			size_t dcIndex = 0;
			for (size_t i = 0; i != _renderData.mStaticMeshList.size(); ++i)
			{
				RStaticMeshProxy& meshProxy = _renderData.mStaticMeshList[i];

				RMesh& Mesh = meshProxy.mMesh->Get_RenderThread();

				for (size_t j = 0; j != Mesh.mSubMeshRange.size(); ++j)
				{
					SRange subMeshRange = Mesh.mSubMeshRange[j];

					_commandList->SetGraphicsPipelineState(mPipelineState);
					_commandList->SetGraphicsRootConstantBuffer(0, mDynamicConstantBuffer.GetRDIResource(), dcIndex * 256 + 256);
					_commandList->SetGraphicsRootConstantBuffer(1, mDynamicConstantBuffer.GetRDIResource(), 0);

					IRDIRenderTargetView* rtvs[] = { _canvas.mTexture->GetRTV(0) };
					_commandList->OMSetRenderTargets(1, rtvs, nullptr);
					Math::SFloatBox viewport = Math::SFloatBox(0.f, 0.f, 0.f, canvasTextureDesc.mSizeX * 1.f, canvasTextureDesc.mSizeY * 1.f, 1.f);
					Math::SIntRect scissorRect = Math::SIntRect(0, 0, canvasTextureDesc.mSizeX, canvasTextureDesc.mSizeY);
					_commandList->RSSetViewports(1, &viewport);
					_commandList->RSSetScissorRects(1, &scissorRect);

					IRDIVertexBufferView* vbvList[] = { Mesh.mVertexBuffer[EnumToInt(EVertexSemantic::Position)]->GetVBV() };

					_commandList->IASetVertexBuffer(0, 1, vbvList);
					_commandList->IASetIndexBuffer(Mesh.mIndexBuffer->GetIBV());
					_commandList->IASetPrimitiveTopology(ERDIPrimitiveTopology::TRIANGLELIST);

					_commandList->DrawIndexedInstanced(static_cast<uint32_t>(subMeshRange.GetSize()), 1, static_cast<uint32_t>(subMeshRange.GetBegin()), 0);

					++dcIndex;
				}
			}

			_commandList->TranstionResourceState(_canvas.mTexture, ERDIResourceState::RenderTarget, ERDIResourceState::Common);
		}, { updateCbTask }
	);

	_renderContext.ExecuteRenderGraph();
}

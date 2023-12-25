#include "Render/RenderContext.h"
#include "Renderer/BaseRenderer.h"
#include "RDI/Interface/RDICommandList.h"
#include "Render/RenderProxy/Resource/RenderResource.h"
#include "Render/RenderProxy/World/WorldProxy.h"
#include "RDI/Interface/RDIDevice.h"
#include "RDI/Interface/RDIShader.h"

void RBaseRenderer::Init(SRenderContext& _renderContext) noexcept
{
	SBlob ShaderVSCodeBuffer;
	{
		FILE* fp;
		fopen_s(&fp, "D:/Project/SevenEngine/Shaders/hlslvs.shader", "rb");
		fseek(fp, 0, SEEK_END);
		long len = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		ShaderVSCodeBuffer.ResizeBlob(len);
		fread(ShaderVSCodeBuffer.GetBuffer(), 1, len, fp);
		fclose(fp);
	}

	SBlob ShaderPSCodeBuffer;
	{
		FILE* fp;
		fopen_s(&fp, "D:/Project/SevenEngine/Shaders/hlslps.shader", "rb");
		fseek(fp, 0, SEEK_END);
		long len = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		ShaderPSCodeBuffer.ResizeBlob(len);
		fread(ShaderPSCodeBuffer.GetBuffer(), 1, len, fp);
		fclose(fp);
	}

	SRDIRootSignatureDesc rootSignatureDesc;
	rootSignatureDesc.mRootParameters.resize(2);

	rootSignatureDesc.mRootParameters[0].mType = ERDIRootParameterType::CBV;
	rootSignatureDesc.mRootParameters[0].mRegisterSpace = 0;
	rootSignatureDesc.mRootParameters[0].mCbvRootParameter.mCbvShaderRegister = 0;
	
	rootSignatureDesc.mRootParameters[1].mType = ERDIRootParameterType::CBV;
	rootSignatureDesc.mRootParameters[1].mRegisterSpace = 0;
	rootSignatureDesc.mRootParameters[1].mCbvRootParameter.mCbvShaderRegister = 1;

	mRootSignature = _renderContext.GetDevice()->CreateRootSignature(&rootSignatureDesc, nullptr);

	SRDIErrorInfo vserrror;
	SRDIErrorInfo pserrror;

	IRDIVertexShader* vertexShader = _renderContext.GetDevice()->CreateVertexShader(ShaderVSCodeBuffer, nullptr, &vserrror);
	IRDIPixelShader* pixelShader = _renderContext.GetDevice()->CreatePixelShader(ShaderPSCodeBuffer, nullptr, &pserrror);

	SRDIVertexInputElememt VertexInputElement;
	VertexInputElement.mSemanticName = "POSITION";
	VertexInputElement.mFormat = ERDIPixelFormat::R32G32B32A32_FLOAT;
	VertexInputElement.mAlignedByteOffset = 0;

	SRDIVertexInputLayoutDesc VertexInputLayoutDesc;
	VertexInputLayoutDesc.mInputElements.push_back(VertexInputElement);

	IRDIInputLayout* inputLayout = _renderContext.GetDevice()->CreateInputLayout(&VertexInputLayoutDesc);

	SRDIGraphicsPipelineState PipelineState;
	PipelineState.mVertexShader = vertexShader;
	PipelineState.mPixelShader = pixelShader;

	PipelineState.mInputLayout = inputLayout;
	PipelineState.mPrimitiveTopologyType = ERDIPrimitiveTopologyType::TRIANGLE;
	PipelineState.mRenderTargetFormat[0] = ERDIPixelFormat::R8G8B8A8_UNORM;
	PipelineState.mRootSignature = mRootSignature;

	mPipelineState = _renderContext.GetDevice()->CreateGraphicsPipelineState(&PipelineState);

	vertexShader->Release();
	pixelShader->Release();
}

void RBaseRenderer::Render(RWorld& _renderData, RCamera& _camera, RTexture2D& _canvas, SRenderContext& _renderContext) noexcept
{
	return;
	SRDITexture2DResourceDesc canvasTextureDesc;
	_canvas.mTexture->GetDesc(&canvasTextureDesc);

	Math::SFloat4x4 projMatrix = Math::CalcPerspectiveProjectMatrix(_camera.mFov, _camera.mNearClip, _camera.mFarClip, canvasTextureDesc.mSizeX, canvasTextureDesc.mSizeY);
	Math::SFloat4x4 viewMatrix = Math::CalcTransformViewMatrix(_camera.mPosition, _camera.mRotation);

	viewMatrix = Math::TransposeMatrix(viewMatrix);
	projMatrix = Math::TransposeMatrix(projMatrix);

	if (mDynamicConstantBuffer.GetDesc().mBufferSize / 256 < _renderData.mStaticMeshList.size())
	{
		_renderContext.SyncToGpuFrameEnd();

		SRDIBufferResourceDesc desc;
		desc.mResourceUsage = ERDIResourceUsage::ConstantBuffer;
		desc.mBufferSize = _renderData.mStaticMeshList.size() * 256 + 256;

		mDynamicConstantBuffer = RDynamicGPUBuffer::ConstructDynamicGPUResource(_renderContext.GetDevice(), &desc);
	}

	void* data;
	mDynamicConstantBuffer.Map(&data);

	uint8_t* dataPtr = static_cast<uint8_t*>(data);

	//memcpy_s(SRange(0, 256).GetBuffer(data), sizeof(Math::SFloat4x4), viewMatrix.GetData(), sizeof(Math::SFloat4x4));
	//memcpy_s(SRange(sizeof(Math::SFloat4x4), 256).GetBuffer(data), sizeof(Math::SFloat4x4), projMatrix.GetData(), sizeof(Math::SFloat4x4));

	for (size_t i = 0; i != _renderData.mStaticMeshList.size(); ++i)
	{
		
	}

	mDynamicConstantBuffer.Unmap();

	_renderContext.AddRenderTask(u8"CB 更新", [=](IRDICommandList* _commandList)
		{
			mDynamicConstantBuffer.Upload(_commandList);
		}
	);

	_renderContext.AddRenderTask(u8"前向绘制", [=](IRDICommandList* _commandList)
		{
			_commandList->ClearRenderTargetView(_canvas.mTexture->GetRTV(0));

			_commandList->SetGraphicsPipelineState(mPipelineState);
			_commandList->SetGraphicsRootSignature(mRootSignature);
			//_commandList->SetGraphicsRootConstantBuffer(0, mDynamicConstantBuffer.GetRDIResource(), );
			_commandList->SetGraphicsRootConstantBuffer(1, mDynamicConstantBuffer.GetRDIResource(), 0);
		}
	);

	_renderContext.SyncToGpuFrameEnd();
	_renderContext.ExecuteRenderGraph();
}

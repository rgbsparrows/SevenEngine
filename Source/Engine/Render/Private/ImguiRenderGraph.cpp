#include "DeferFrameTask.h"
#include "Core/Math/Type.h"
#include "ImguiRenderGraph.h"
#include "Render/RenderContext.h"
#include "RDI/RDIFunctionHelper.h"
#include "RDI/Interface/RDIShader.h"
#include "RDI/Interface/RDISwapChain.h"
#include "RDI/Interface/RDIInputLayout.h"
#include "RDI/Interface/RDIRootSignature.h"
#include "RDI/Interface/RDIPipelineState.h"
#include "Render/RenderProxy/RenderProxy.h"

void RImguiRenderGraph::Init(SRenderContext& _renderContext) noexcept
{
	const char* vertexShader =
		"cbuffer MainCb : register(b0)										\n"
		"{																	\n"
		"	float4x4 ProjectionMatrix;										\n"
		"};																	\n"
		"																	\n"
		"struct VsInput														\n"
		"{																	\n"
		"	float2 pos : POSITION;											\n"
		"	float2 uv  : UV;												\n"
		"	float4 col : COLOR;												\n"
		"};																	\n"
		"																	\n"
		"struct VsOutput													\n"
		"{																	\n"
		"	float2 uv  : UV;												\n"
		"	float4 col : COLOR;												\n"
		"	float4 pos : SV_POSITION;										\n"
		"};																	\n"
		"																	\n"
		"VsOutput VsMain(VsInput _input)									\n"
		"{																	\n"
		"	VsOutput output;												\n"
		"	output.pos = mul(ProjectionMatrix, float4(_input.pos, 0, 1));	\n"
		"	output.uv = _input.uv;											\n"
		"	output.col = _input.col;										\n"
		"	return output;													\n"
		"}"
		;

	const char* pixelShader =
		"SamplerState sampler0 : register(s0);								\n"
		"Texture2D texture0 : register(t0);									\n"
		"																	\n"
		"struct PsInput														\n"
		"{																	\n"
		"	float2 uv  : UV;												\n"
		"	float4 col : COLOR;												\n"
		"};																	\n"
		"																	\n"
		"float4 PsMain(PsInput _input) : SV_Target							\n"
		"{																	\n"
		"	return _input.col * texture0.Sample(sampler0, _input.uv);		\n"
		"}																	\n"
		;

	SConstBufferView vsBuffer(vertexShader, strlen(vertexShader));
	SConstBufferView psBuffer(pixelShader, strlen(pixelShader));

	SRDIErrorInfo errorInfo;
	IRDIVertexShader* vs = _renderContext.GetDevice()->CreateVertexShader(vsBuffer, nullptr, &errorInfo);
	IRDIPixelShader* ps = _renderContext.GetDevice()->CreatePixelShader(psBuffer, nullptr, &errorInfo);

	SRDIRootParameter cbParameter;
	cbParameter.mType = ERDIRootParameterType::CBV;
	cbParameter.mShaderVisibility = ERDIShaderVisibility::VERTEX_VISABLE;
	cbParameter.mRegisterSpace = 0;
	cbParameter.mCbvRootParameter.mCbvShaderRegister = 0;

	SRDIRootParameter texParameter;
	texParameter.mType = ERDIRootParameterType::DescriptorTable;
	texParameter.mShaderVisibility = ERDIShaderVisibility::PIXEL_VISABLE;
	texParameter.mRegisterSpace = 0;
	texParameter.mDescriptorTableRootParameter.mSrvStartShaderRegister = 0;
	texParameter.mDescriptorTableRootParameter.mSrvNumDescriptors = 1;
	texParameter.mDescriptorTableRootParameter.mUavStartShaderRegister = 0;
	texParameter.mDescriptorTableRootParameter.mUavNumDescriptors = 0;

	SRDIStaticSamplerDesc staticSampler;
	staticSampler.mFilter = ERDIFilter::MIN_POINT | ERDIFilter::MAG_POINT | ERDIFilter::MIP_POINT;
	staticSampler.mAddressU = ERDIAddressMode::MIRROR;
	staticSampler.mAddressV = ERDIAddressMode::MIRROR;
	staticSampler.mAddressW = ERDIAddressMode::MIRROR;
	staticSampler.mMipLODBias = 0.f;
	staticSampler.mMaxAnisotropy = 0;
	staticSampler.mComparisonFunc = ERDIComparisonFunc::ALWAYS;
	staticSampler.mBorderColor = ERDIStaticBorderColor::TRANSPARENT_BLACK;
	staticSampler.mMinLod = 0.f;
	staticSampler.mMaxLod = 0.f;
	staticSampler.mShaderRegister = 0;
	staticSampler.mRegisterSpace = 0;
	staticSampler.mShaderVisibility = ERDIShaderVisibility::PIXEL_VISABLE;

	SRDIRootSignatureDesc rootSignatureDesc;
	rootSignatureDesc.mRootParameters.push_back(cbParameter);
	rootSignatureDesc.mRootParameters.push_back(texParameter);
	rootSignatureDesc.mStaticSamplerDescs.push_back(staticSampler);

	mImguiRootSignature = _renderContext.GetDevice()->CreateRootSignature(&rootSignatureDesc, nullptr);

	SRDIVertexInputElememt inputLayoutElement[3];

	inputLayoutElement[0].mSemanticName = "POSITION";
	inputLayoutElement[0].mFormat = ERDIPixelFormat::R32G32_FLOAT;
	inputLayoutElement[0].mAlignedByteOffset = 0;

	inputLayoutElement[1].mSemanticName = "UV";
	inputLayoutElement[1].mFormat = ERDIPixelFormat::R32G32_FLOAT;
	inputLayoutElement[1].mAlignedByteOffset = 8;

	inputLayoutElement[2].mSemanticName = "COLOR";
	inputLayoutElement[2].mFormat = ERDIPixelFormat::R8G8B8A8_UNORM;
	inputLayoutElement[2].mAlignedByteOffset = 16;

	SRDIVertexInputLayoutDesc inputLayoutDesc;
	inputLayoutDesc.mInputElements.insert(inputLayoutDesc.mInputElements.begin(), std::begin(inputLayoutElement), std::end(inputLayoutElement));

	IRDIInputLayout* inputLayout = _renderContext.GetDevice()->CreateInputLayout(&inputLayoutDesc);

	SRDIGraphicsPipelineState pipelineStateDesc;
	pipelineStateDesc.mRootSignature = mImguiRootSignature;
	pipelineStateDesc.mVertexShader = vs;
	pipelineStateDesc.mPixelShader = ps;
	pipelineStateDesc.mInputLayout = inputLayout;
	pipelineStateDesc.mPrimitiveTopologyType = ERDIPrimitiveTopologyType::TRIANGLE;
	pipelineStateDesc.mBlendState.mBlendEnable = true;
	pipelineStateDesc.mRasterizationState.mCullMode = ERDICullMode::None;
	pipelineStateDesc.mDepthStencilState.mDepthTestEnable = false;
	pipelineStateDesc.mDepthStencilState.mDepthWriteEnable = false;
	pipelineStateDesc.mDepthStencilFormat = ERDIPixelFormat::UNKNOWN;

	pipelineStateDesc.mRenderTargetFormat[0] = ERDIPixelFormat::R8G8B8A8_UNORM;
	mImguiSDRPipelineState = _renderContext.GetDevice()->CreateGraphicsPipelineState(&pipelineStateDesc);

	pipelineStateDesc.mRenderTargetFormat[0] = ERDIPixelFormat::R10G10B10A2_UNORM;
	mImguiHDR10PipelineState = _renderContext.GetDevice()->CreateGraphicsPipelineState(&pipelineStateDesc);

	pipelineStateDesc.mRenderTargetFormat[0] = ERDIPixelFormat::R16G16B16A16_FLOAT;
	mImguiHDR1000PipelineState = _renderContext.GetDevice()->CreateGraphicsPipelineState(&pipelineStateDesc);

	vs->Release();
	ps->Release();
	inputLayout->Release();
}

void RImguiRenderGraph::Clear(SRenderContext& _renderContext) noexcept
{
	mImguiRootSignature->Release();
	mImguiSDRPipelineState->Release();
	mImguiHDR10PipelineState->Release();
	mImguiHDR1000PipelineState->Release();

	mDynamicConstantBuffer = RDynamicGPUBuffer();

	for (size_t i = 0; i != mDynamicIndexBufferList.size(); ++i)
		mDynamicIndexBufferList[i] = RDynamicGPUBuffer();

	for(size_t i = 0; i != mDynamicVertexBufferList.size(); ++i)
		mDynamicVertexBufferList[i] = RDynamicGPUBuffer();
}

void RImguiRenderGraph::Render(std::vector<RRenderWindowInfo>& _renderWindowInfo, SRenderContext& _renderContext) noexcept
{
	if (_renderWindowInfo.empty())
		return;

	if (mDynamicConstantBuffer.GetDesc().mBufferSize < 256 * _renderWindowInfo.size())
	{
		_renderContext.SyncToGpuFrameEnd();

		SRDIBufferResourceDesc cbDesc;
		cbDesc.mHeapType = ERDIHeapType::Default;
		cbDesc.mResourceUsage = ERDIResourceUsage::ConstantBuffer;
		cbDesc.mBufferSize = 256 * _renderWindowInfo.size() * 2;

		mDynamicConstantBuffer = RDynamicGPUBuffer::ConstructDynamicGPUResource(_renderContext.GetDevice(), &cbDesc);
	}

	void* cbData = nullptr;
	mDynamicConstantBuffer.Map(&cbData);
	for (size_t i = 0; i != _renderWindowInfo.size(); ++i)
	{
		auto& drawData = _renderWindowInfo[i].mImguiDrawData->Get_RenderThread();
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
	mDynamicConstantBuffer.Unmap();

	SRenderTaskIdentify updateImguiCbTask = _renderContext.AddRenderTask(u8"更新ImguiCb", [=](IRDICommandList* _commandList)
		{
			mDynamicConstantBuffer.Upload(_commandList);
		});

	for (size_t i = 0; i != _renderWindowInfo.size(); ++i)
	{
		RImguiDrawData& drawData = _renderWindowInfo[i].mImguiDrawData->Get_RenderThread();
		IRDISwapChain* swapChain = _renderWindowInfo[i].mSwapChain->Get_RenderThread().mSwapChain;

		if (mDynamicIndexBufferList.size() <= i)
			mDynamicIndexBufferList.push_back(RDynamicGPUBuffer());

		if (mDynamicVertexBufferList.size() <= i)
			mDynamicVertexBufferList.push_back(RDynamicGPUBuffer());

		if (mDynamicIndexBufferList[i].GetDesc().mBufferSize < drawData.mIndexBuffer.size() * sizeof(uint32_t))
		{
			_renderContext.SyncToGpuFrameEnd();

			SRDIBufferResourceDesc ibDesc;
			ibDesc.mHeapType = ERDIHeapType::Default;
			ibDesc.mResourceUsage = ERDIResourceUsage::IndexBuffer;
			ibDesc.mBufferSize = drawData.mIndexBuffer.size() * sizeof(uint32_t) * 2;
			ibDesc.mElementStride = sizeof(uint32_t);

			mDynamicIndexBufferList[i] = RDynamicGPUBuffer::ConstructDynamicGPUResource(_renderContext.GetDevice(), &ibDesc);
		}

		if (mDynamicVertexBufferList[i].GetDesc().mBufferSize < drawData.mVertexBuffer.size() * sizeof(RImguiVertex))
		{
			_renderContext.SyncToGpuFrameEnd();

			SRDIBufferResourceDesc vbDesc;
			vbDesc.mHeapType = ERDIHeapType::Default;
			vbDesc.mResourceUsage = ERDIResourceUsage::VertexBuffer;
			vbDesc.mBufferSize = drawData.mVertexBuffer.size() * sizeof(RImguiVertex) * 2;
			vbDesc.mElementStride = sizeof(RImguiVertex);

			mDynamicVertexBufferList[i] = RDynamicGPUBuffer::ConstructDynamicGPUResource(_renderContext.GetDevice(), &vbDesc);
		}

		void* ibData = nullptr;
		mDynamicIndexBufferList[i].Map(&ibData);
		memcpy_s(ibData, drawData.mIndexBuffer.size() * sizeof(uint32_t), drawData.mIndexBuffer.data(), drawData.mIndexBuffer.size() * sizeof(uint32_t));
		mDynamicIndexBufferList[i].Unmap();

		void* vbData = nullptr;
		mDynamicVertexBufferList[i].Map(&vbData);
		memcpy_s(vbData, drawData.mVertexBuffer.size() * sizeof(RImguiVertex), drawData.mVertexBuffer.data(), drawData.mVertexBuffer.size() * sizeof(RImguiVertex));
		mDynamicVertexBufferList[i].Unmap();

		SRenderTaskIdentify updateImguiVBTask = _renderContext.AddRenderTask(u8"Render Imgui", [=, &drawData](IRDICommandList* _commandList)
			{
				mDynamicIndexBufferList[i].Upload(_commandList);
				mDynamicVertexBufferList[i].Upload(_commandList);

				_commandList->TranstionResourceState(swapChain->GetRenderTarget(), ERDIResourceState::Common, ERDIResourceState::RenderTarget);

				IRDIRenderTargetView* renderTargetView = swapChain->GetRenderTarget()->GetRTV(0);
				_commandList->OMSetRenderTargets(1, &renderTargetView, nullptr);
				_commandList->ClearRenderTargetView(renderTargetView);

				SRDISwapChainDesc swapChainDesc;
				swapChain->GetDesc(&swapChainDesc);
				switch (swapChainDesc.mPixelFormat)
				{
				case ERDIPixelFormat::R8G8B8A8_UNORM:
					_commandList->SetGraphicsPipelineState(mImguiSDRPipelineState);
					break;
				case ERDIPixelFormat::R10G10B10A2_UNORM:
					_commandList->SetGraphicsPipelineState(mImguiHDR10PipelineState);
					break;
				case ERDIPixelFormat::R16G16B16A16_FLOAT:
					_commandList->SetGraphicsPipelineState(mImguiHDR1000PipelineState);
					break;
				default:
					CHECK(false && "不恰当的PipelineState");
				}

				_commandList->SetGraphicsRootSignature(mImguiRootSignature);

				IRDIVertexBufferView* vbv = mDynamicVertexBufferList[i].GetRDIResource()->GetVBV();
				_commandList->IASetVertexBuffer(0, 1, &vbv);
				_commandList->IASetIndexBuffer(mDynamicIndexBufferList[i].GetRDIResource()->GetIBV());
				_commandList->IASetPrimitiveTopology(ERDIPrimitiveTopology::TRIANGLELIST);

				Math::SFloatBox viewportBox = Math::SFloatBox(0, 0, 0, drawData.mDisplaySize[0], drawData.mDisplaySize[1], 1);
				_commandList->RSSetViewports(1, &viewportBox);

				_commandList->SetGraphicsRootConstantBuffer(0, mDynamicConstantBuffer.GetRDIResource(), i * 256);
				for (RImguiDrawCmd& cmd : drawData.mCmdBuffer)
				{
					RImguiTexture2D& imguiTexture = cmd.mTextureId->Get_RenderThread();
					_commandList->SetGraphicsRootDescriptorTable(1, imguiTexture.mDescriptorHeapRange);
					_commandList->RSSetScissorRects(1, &cmd.mClipRect);
					_commandList->DrawIndexedInstanced(cmd.mVertexCount, 1, cmd.mIndexOffset, cmd.mVertexOffset);
				}

				_commandList->TranstionResourceState(swapChain->GetRenderTarget(), ERDIResourceState::RenderTarget, ERDIResourceState::Present);

			}, { updateImguiCbTask });
	}

	_renderContext.ExecuteRenderGraph();

	for (uint32_t i = 0; i < _renderWindowInfo.size(); ++i)
	{
		_renderWindowInfo[i].mSwapChain->Get_RenderThread().mSwapChain->Present(_renderWindowInfo[i].mEnableVSync);
	}
}

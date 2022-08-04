#include "RenderResource.h"
#include "RDI/Interface/RDIDevice.h"
#include "Core/Container/Blob.h"
#include "RDI/RDIFunctionHelper.h"
#include "RDI/Interface/RDIShader.h"
#include "RDI/Interface/RDIRootSignature.h"
#include "RDI/Interface/RDIInputLayout.h"
#include "RDI/Interface/RDIPipelineState.h"
#include "RDI/Interface/RDIResource.h"

void RStaticRenderResource::Init(IRDIDevice* _device) noexcept
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
		"VsOutput main(VsInput _input)										\n"
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
		"float4 main(PsInput _input) : SV_Target							\n"
		"{																	\n"
		"	return _input.col * texture0.Sample(sampler0, _input.uv);		\n"
		"}																	\n"
		;

	SBufferView vsBuffer(vertexShader, strlen(vertexShader));
	SBufferView psBuffer(pixelShader, strlen(pixelShader));

	SRDIErrorInfo errorInfo;
	IRDIVertexShader* vs = _device->CreateVertexShader(vsBuffer, nullptr, &errorInfo);
	IRDIPixelShader* ps = _device->CreatePixelShader(psBuffer, nullptr, &errorInfo);

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

	mImguiRootSignature = _device->CreateRootSignature(&rootSignatureDesc, nullptr);

	SRDIVertexInputElememt inputLayoutElement[3];

	inputLayoutElement[0].mSemanticName = L"POSITION";
	inputLayoutElement[0].mFormat = ERDIPixelFormat::R32G32_FLOAT;
	inputLayoutElement[0].mAlignedByteOffset = 0;

	inputLayoutElement[1].mSemanticName = L"UV";
	inputLayoutElement[1].mFormat = ERDIPixelFormat::R32G32_FLOAT;
	inputLayoutElement[1].mAlignedByteOffset = 8;

	inputLayoutElement[2].mSemanticName = L"COLOR";
	inputLayoutElement[2].mFormat = ERDIPixelFormat::R8G8B8A8_UNORM;
	inputLayoutElement[2].mAlignedByteOffset = 16;

	SRDIVertexInputLayoutDesc inputLayoutDesc;
	inputLayoutDesc.mInputElements.insert(inputLayoutDesc.mInputElements.begin(), std::begin(inputLayoutElement), std::end(inputLayoutElement));
	
	IRDIInputLayout* inputLayout = _device->CreateInputLayout(&inputLayoutDesc);

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
	mImguiSDRPipelineState = _device->CreateGraphicsPipelineState(&pipelineStateDesc);

	pipelineStateDesc.mRenderTargetFormat[0] = ERDIPixelFormat::R10G10B10A2_UNORM;
	mImguiHDR10PipelineState = _device->CreateGraphicsPipelineState(&pipelineStateDesc);

	pipelineStateDesc.mRenderTargetFormat[0] = ERDIPixelFormat::R16G16B16A16_FLOAT;
	mImguiHDR1000PipelineState = _device->CreateGraphicsPipelineState(&pipelineStateDesc);

	vs->Release();
	ps->Release();
	inputLayout->Release();
}

void RStaticRenderResource::Clear() noexcept
{
	mImguiSDRPipelineState->Release();
	mImguiHDR10PipelineState->Release();
	mImguiHDR1000PipelineState->Release();
	mImguiRootSignature->Release();

	if (mConstantBuffer)
		mConstantBuffer->Release();
}

void RFrameRenderResource::Clear() noexcept
{
	if (mConstantUploadBuffer)
		mConstantUploadBuffer->Release();
}

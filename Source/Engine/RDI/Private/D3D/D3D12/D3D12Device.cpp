#include "D3D/D3DUtil.h"
#include "Core/Localize.h"
#include "Core/Path/BasicPath.h"
#include "D3D/D3D12/D3D12Device.h"
#include "D3D/D3D12/D3D12Factory.h"
#include "D3D/Warper/D3DImplWarper.h"
#include "D3D/Warper/D3DEnumConvertor.h"
#include "D3D/D3D12/Warper/D3D12ImplWarper.h"
#include "D3D/D3D12/Warper/D3D12EnumConvertor.h"
#include "D3D/D3D12/Warper/D3D12ImplWarperHelper.h"

void SD3D12Device::Init(void* _nativePtr, SD3D12Adapter* _adapter, SD3D12Factory* _factory) noexcept
{
	{
		mD3D12DeviceNativePtr = _nativePtr;
		mAdapter = _adapter;
	}

	//CommandQuue
	{
		void* commandQueueNativePtr = nullptr;
		VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12CreateCommandQueue_D3D12Impl(GetNativePtr(), &commandQueueNativePtr));
		void* fenceNativePtr = nullptr;
		VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12CreateFence_D3D12Impl(GetNativePtr(), 0, &fenceNativePtr));
		mCommandQueue.Init(commandQueueNativePtr, fenceNativePtr);
	}

	//DescriptorHeaps
	{
		void* shaderVisibleCbvSrvUavDescriptorHeap = nullptr;
		void* shaderVisibleSamplerViewDescriptorHeap = nullptr;
		void* rtvDescriptorHeap = nullptr;
		void* dsvDescriptorHeap = nullptr;
		void* srvDescriptorHeap = nullptr;
		void* uavDescriptorHeap = nullptr;
		void* samplerViewDescriptorHeap = nullptr;

		VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12CreateDescriptorHeap_D3D12Impl(GetNativePtr(), EnumToInt(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), true, ShaderVisibleCbvSrvUavDescriptorCount, &shaderVisibleCbvSrvUavDescriptorHeap));
		VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12CreateDescriptorHeap_D3D12Impl(GetNativePtr(), EnumToInt(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER), true, ShaderVisibleSamplerViewDescriptorCount, &shaderVisibleSamplerViewDescriptorHeap));
		VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12CreateDescriptorHeap_D3D12Impl(GetNativePtr(), EnumToInt(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV), false, RTVDescriptorCount, &rtvDescriptorHeap));
		VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12CreateDescriptorHeap_D3D12Impl(GetNativePtr(), EnumToInt(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV), false, DSVDescriptorCount, &dsvDescriptorHeap));
		VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12CreateDescriptorHeap_D3D12Impl(GetNativePtr(), EnumToInt(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), false, SRVDescriptorCount, &srvDescriptorHeap));
		VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12CreateDescriptorHeap_D3D12Impl(GetNativePtr(), EnumToInt(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), false, UAVDescriptorCount, &uavDescriptorHeap));
		VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12CreateDescriptorHeap_D3D12Impl(GetNativePtr(), EnumToInt(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER), false, SamplerViewDescriptorCount, &samplerViewDescriptorHeap));

		mShaderVisibleDescriptorHeap.Init(shaderVisibleCbvSrvUavDescriptorHeap, ShaderVisibleCbvSrvUavDescriptorCount, shaderVisibleSamplerViewDescriptorHeap, ShaderVisibleSamplerViewDescriptorCount, this);
		mDescriptorHeap.Init(rtvDescriptorHeap, RTVDescriptorCount, dsvDescriptorHeap, DSVDescriptorCount, srvDescriptorHeap, SRVDescriptorCount, uavDescriptorHeap, UAVDescriptorCount, samplerViewDescriptorHeap, SamplerViewDescriptorCount, this);
	}

	//DescriptorHandleIncrementSize
	{
		for (int32_t i = 0; i != EnumToInt(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES); ++i)
		{
			D3D12APIWarp_Impl::D3D12GetDescriptorHandleIncrementSize_D3D12Impl(GetNativePtr(), i, &mDescriptorHandleIncrement[i]);
		}
	}

	//ShaderCompileInfo
	{
		mShaderCompileFlag = EShaderCompileFlag::D3DCOMPILE_AVOID_FLOW_CONTROL | EShaderCompileFlag::D3DCOMPILE_ENABLE_STRICTNESS | EShaderCompileFlag::D3DCOMPILE_WARNINGS_ARE_ERRORS;

		if (SProgramConfiguation::UseDebugShader())
			mShaderCompileFlag |= EShaderCompileFlag::D3DCOMPILE_DEBUG | EShaderCompileFlag::D3DCOMPILE_SKIP_OPTIMIZATION | EShaderCompileFlag::D3DCOMPILE_DEBUG_NAME_FOR_SOURCE | EShaderCompileFlag::D3DCOMPILE_DEBUG_NAME_FOR_BINARY;
		else
			mShaderCompileFlag |= EShaderCompileFlag::D3DCOMPILE_OPTIMIZATION_LEVEL3;

		mSearchShaderHeaderFileFunc = [](const std::filesystem::path& _includeFile)
		{
			auto filePath = SBasicPath::GetEngineShaderPath() / _includeFile;
			if (std::filesystem::exists(filePath))
				return filePath;

			filePath = SBasicPath::GetProjectShaderPath() / _includeFile;
			if (std::filesystem::exists(filePath))
				return filePath;

			return std::filesystem::path();
		};
	}
}

IRDICommandList* SD3D12Device::GetCommandList(uint16_t _commandListIndex) noexcept
{
	CHECK(_commandListIndex < mCommandList.size());

	return &mCommandList[_commandListIndex];
}

IRDISwapChain* SD3D12Device::CreateSwapChain(const SRDISwapChainDesc* _swapChainDesc) noexcept
{
	CHECK(_swapChainDesc->mBufferCount <= D3D12_BACKBUFFER_COUNT);

	DXGI_SWAP_CHAIN_DESC desc;

	desc.BufferDesc.Width = _swapChainDesc->mWidth;
	desc.BufferDesc.Height = _swapChainDesc->mHeight;
	desc.BufferDesc.RefreshRate.Numerator = _swapChainDesc->mRefreshRate.mNumerator;
	desc.BufferDesc.RefreshRate.Denominator = _swapChainDesc->mRefreshRate.mDenominator;
	desc.BufferDesc.Format = ConvertPixelFormatToD3D(_swapChainDesc->mPixelFormat);
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = ConvertScalingModeToD3D(_swapChainDesc->mScalingMode);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE::DXGI_USAGE_BACK_BUFFER;
	desc.BufferCount = _swapChainDesc->mBufferCount;
	desc.OutputWindow = _swapChainDesc->mOutputWindow;
	desc.Windowed = _swapChainDesc->mIsWindowed;
	desc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_NONE;

	void* swapchainNativePtr = nullptr;
	VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12CreateSwapChain_D3D12Impl(mFactory->GetNativePtr(), GetNativePtr(), &desc, &swapchainNativePtr));

	SD3D12SwapChain* swapChain = mSwapChainPool.AllocateElement();
	swapChain->Init(swapchainNativePtr, _swapChainDesc, this, mAdapter);
	return swapChain;
}

void SD3D12Device::EnsureCommandListCount(size_t _commandListCount) noexcept
{
	if (mCommandList.size() >= _commandListCount)
		return;

	for (size_t i = mCommandList.size(); i != _commandListCount; ++i)
	{
		void* commandListNativePtr = nullptr;
		void* commandAllcatorNativePtr = nullptr;
		VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12CreateCommandList_D3D12Impl(GetNativePtr(), &commandAllcatorNativePtr, &commandListNativePtr));

		mCommandList.push_back(SD3D12CommandList());
		mCommandList.back().Init(commandAllcatorNativePtr, commandListNativePtr, this);
	}
}

IRDIInputLayout* SD3D12Device::CreateInputLayout(const SRDIVertexInputLayoutDesc* _desc) noexcept
{
	SD3D12InputLayout* inputLayout = mInputLayoutPool.AllocateElement();
	inputLayout->Init(_desc);

	return inputLayout;
}

IRDIRootSignature* SD3D12Device::CreateRootSignature(const SRDIRootSignatureDesc* _desc, SRDIErrorInfo* _rootSignatureError) noexcept
{
	static D3D12_DESCRIPTOR_RANGE descriptorRange[30] = {};
	static D3D12_ROOT_PARAMETER rootParameters[10] = {};
	static D3D12_STATIC_SAMPLER_DESC staticSamplerDesc[30] = {};

	D3D12_ROOT_SIGNATURE_DESC desc = {};

	for (size_t i = 0, j = 0; i != _desc->mRootParameters.size(); void())
	{
		auto& _ele = _desc->mRootParameters[i];
		switch (_ele.mType)
		{
		case ERDIRootParameterType::CBV:
			rootParameters[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[i].Descriptor.RegisterSpace = _ele.mRegisterSpace;
			rootParameters[i].Descriptor.ShaderRegister = _ele.mCbvRootParameter.mCbvShaderRegister;
			break;
		case ERDIRootParameterType::DescriptorTable:
		{
			CHECK(_ele.mDescriptorTableRootParameter.mSrvNumDescriptors != 0 || _ele.mDescriptorTableRootParameter.mUavNumDescriptors != 0);

			uint32_t rangeCount = 0;

			rootParameters[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[i].DescriptorTable.pDescriptorRanges = &descriptorRange[j];
			if (_ele.mDescriptorTableRootParameter.mSrvNumDescriptors != 0)
			{
				descriptorRange[j].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
				descriptorRange[j].RegisterSpace = _ele.mRegisterSpace;
				descriptorRange[j].NumDescriptors = _ele.mDescriptorTableRootParameter.mSrvNumDescriptors;
				descriptorRange[j].BaseShaderRegister = _ele.mDescriptorTableRootParameter.mSrvStartShaderRegister;
				descriptorRange[j].OffsetInDescriptorsFromTableStart = 0;
				++j;
				++rangeCount;
			}

			if (_ele.mDescriptorTableRootParameter.mUavNumDescriptors != 0)
			{
				descriptorRange[j].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
				descriptorRange[j].RegisterSpace = _ele.mRegisterSpace;
				descriptorRange[j].NumDescriptors = _ele.mDescriptorTableRootParameter.mUavNumDescriptors;
				descriptorRange[j].BaseShaderRegister = _ele.mDescriptorTableRootParameter.mUavStartShaderRegister;
				descriptorRange[j].OffsetInDescriptorsFromTableStart = _ele.mDescriptorTableRootParameter.mSrvStartShaderRegister;
				++j;
				++rangeCount;
			}
			rootParameters[i].DescriptorTable.NumDescriptorRanges = rangeCount;
			break;
		}
		case ERDIRootParameterType::SamplerTable:
		{
			CHECK(_ele.mSamplerTableRootParameter.mSamplerNumDescriptors != 0);

			rootParameters[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[i].DescriptorTable.pDescriptorRanges = &descriptorRange[j];
			rootParameters[i].DescriptorTable.NumDescriptorRanges = 1;
			descriptorRange[j].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
			descriptorRange[j].RegisterSpace = _ele.mRegisterSpace;
			descriptorRange[j].NumDescriptors = _ele.mSamplerTableRootParameter.mSamplerNumDescriptors;
			descriptorRange[j].BaseShaderRegister = _ele.mSamplerTableRootParameter.mSamplerStartShaderRegister;
			descriptorRange[j].OffsetInDescriptorsFromTableStart = 0;
			++j;
			break;
		}
		}
		rootParameters[i].ShaderVisibility = ConvertShaderVisibilityToD3D12(_ele.mShaderVisibility);
	}

	for (size_t i = 0; i != _desc->mStaticSamplerDescs.size(); ++i)
	{
		auto& _ele = _desc->mStaticSamplerDescs[i];

		staticSamplerDesc[i].Filter = ConvertFilterToD3D12(_ele.mFilter);
		staticSamplerDesc[i].AddressU = ConvertAddressModeToD3D12(_ele.mAddressU);
		staticSamplerDesc[i].AddressV = ConvertAddressModeToD3D12(_ele.mAddressV);
		staticSamplerDesc[i].AddressW = ConvertAddressModeToD3D12(_ele.mAddressW);
		staticSamplerDesc[i].MipLODBias = _ele.mMipLODBias;
		staticSamplerDesc[i].MaxAnisotropy = _ele.mMaxAnisotropy;
		staticSamplerDesc[i].ComparisonFunc = ConvertComparisonFuncToD3D12(_ele.mComparisonFunc);
		staticSamplerDesc[i].BorderColor = ConvertStaticBorderColorToD3D12(_ele.mBorderColor);
		staticSamplerDesc[i].MinLOD = _ele.mMinLod;
		staticSamplerDesc[i].MaxLOD = _ele.mMaxLod;
		staticSamplerDesc[i].ShaderRegister = _ele.mShaderRegister;
		staticSamplerDesc[i].RegisterSpace = _ele.mRegisterSpace;
		staticSamplerDesc[i].ShaderVisibility = ConvertShaderVisibilityToD3D12(_ele.mShaderVisibility);
	}

	desc.NumParameters = static_cast<uint32_t>( _desc->mRootParameters.size());
	desc.pParameters = rootParameters;
	desc.NumStaticSamplers = static_cast<uint32_t>(_desc->mStaticSamplerDescs.size());
	desc.pStaticSamplers = staticSamplerDesc;
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_NONE;

	std::vector<uint8_t> serlizedBlob;
	std::vector<uint8_t> errorBlob;
	uint32_t res = D3D12APIWarp_Impl::D3D12SerializeRootSignature_D3D12Impl(&desc, static_cast<int32_t>(D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1), serlizedBlob, errorBlob);

	IRDIRootSignature* rootSignature = nullptr;

	if (res == 0)
		rootSignature = CreateRootSignature(SBufferView(serlizedBlob.data(), serlizedBlob.size()));
	else
		GenerateErrorInfo(errorBlob, _rootSignatureError);

	return rootSignature;
}

IRDIRootSignature* SD3D12Device::CreateRootSignature(const SBufferView _serializedRootSignatureBlob) noexcept
{
	void* rootSignatureNativePtr = nullptr;
	VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12CreateRootSignature_D3D12Impl(_serializedRootSignatureBlob.GetBuffer(), _serializedRootSignatureBlob.GetBufferSize(), GetNativePtr(), &rootSignatureNativePtr));

	return mRootSignaturePool.AllocateElement(_serializedRootSignatureBlob.GetBuffer(), _serializedRootSignatureBlob.GetBufferSize(), rootSignatureNativePtr);
}

IRDIGraphicsPipelineState* SD3D12Device::CreateGraphicsPipelineState(const SRDIGraphicsPipelineState* _desc) noexcept
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	desc.pRootSignature = static_cast<SD3D12RootSignature*>(_desc->mRootSignature)->GetNativePtr();
	desc.VS.pShaderBytecode = static_cast<SD3D12VertexShader*>(_desc->mVertexShader)->GetCompiledShaderBlob().GetBuffer();
	desc.VS.BytecodeLength = static_cast<SD3D12VertexShader*>(_desc->mVertexShader)->GetCompiledShaderBlob().GetBufferSize();
	desc.HS.pShaderBytecode = static_cast<SD3D12HullShader*>(_desc->mHullShader)->GetCompiledShaderBlob().GetBuffer();
	desc.HS.BytecodeLength = static_cast<SD3D12HullShader*>(_desc->mHullShader)->GetCompiledShaderBlob().GetBufferSize();
	desc.DS.pShaderBytecode = static_cast<SD3D12DomainShader*>(_desc->mDomainShader)->GetCompiledShaderBlob().GetBuffer();
	desc.DS.BytecodeLength = static_cast<SD3D12DomainShader*>(_desc->mDomainShader)->GetCompiledShaderBlob().GetBufferSize();
	desc.GS.pShaderBytecode = static_cast<SD3D12GeometryShader*>(_desc->mGeometryShader)->GetCompiledShaderBlob().GetBuffer();
	desc.GS.BytecodeLength = static_cast<SD3D12GeometryShader*>(_desc->mGeometryShader)->GetCompiledShaderBlob().GetBufferSize();
	desc.PS.pShaderBytecode = static_cast<SD3D12PixelShader*>(_desc->mPixelShader)->GetCompiledShaderBlob().GetBuffer();
	desc.PS.BytecodeLength = static_cast<SD3D12PixelShader*>(_desc->mPixelShader)->GetCompiledShaderBlob().GetBufferSize();
	desc.StreamOutput.pSODeclaration = nullptr;
	desc.StreamOutput.NumEntries = 0;
	desc.StreamOutput.pBufferStrides = nullptr;
	desc.StreamOutput.NumStrides = 0;
	desc.StreamOutput.RasterizedStream = 0;
	desc.BlendState.AlphaToCoverageEnable = false;
	desc.BlendState.IndependentBlendEnable = false;
	desc.BlendState.RenderTarget[0].BlendEnable = _desc->mBlendState.mBlendEnable;
	desc.BlendState.RenderTarget[0].LogicOpEnable = _desc->mBlendState.mLogicOpEnable;
	desc.BlendState.RenderTarget[0].SrcBlend = ConvertBlendFactoryToD3D12(_desc->mBlendState.mSrcBlend);
	desc.BlendState.RenderTarget[0].DestBlend = ConvertBlendFactoryToD3D12(_desc->mBlendState.mDestBlend);
	desc.BlendState.RenderTarget[0].BlendOp = ConvertBlendOperatorToD3D12(_desc->mBlendState.mBlendOperator);
	desc.BlendState.RenderTarget[0].SrcBlendAlpha = ConvertBlendFactoryToD3D12(_desc->mBlendState.mSrcAlphaBlend);
	desc.BlendState.RenderTarget[0].DestBlendAlpha = ConvertBlendFactoryToD3D12(_desc->mBlendState.mDestAlphaBlend);
	desc.BlendState.RenderTarget[0].BlendOpAlpha = ConvertBlendOperatorToD3D12(_desc->mBlendState.mBlendAlphaOperator);
	desc.BlendState.RenderTarget[0].LogicOp = ConvertLogicOperatorToD3D12(_desc->mBlendState.mLogicOperator);
	desc.BlendState.RenderTarget[0].RenderTargetWriteMask = ConvertColorWriteMaskToD3D12(_desc->mBlendState.mColorWriteMask);
	desc.SampleMask = UINT_MAX;
	desc.RasterizerState.FillMode = ConvertFillModeToD3D12(_desc->mRasterizationState.mFillMode);
	desc.RasterizerState.CullMode = ConvertCullModeToD3D12(_desc->mRasterizationState.mCullMode);
	desc.RasterizerState.FrontCounterClockwise = true;
	desc.RasterizerState.DepthBias = _desc->mRasterizationState.mDepthBias;
	desc.RasterizerState.DepthBiasClamp = _desc->mRasterizationState.mDepthBiasClamp;
	desc.RasterizerState.SlopeScaledDepthBias = _desc->mRasterizationState.mSlopeScaledDepthBias;
	desc.RasterizerState.DepthClipEnable = _desc->mRasterizationState.mDepthClipEnable;
	desc.RasterizerState.MultisampleEnable = false;
	desc.RasterizerState.AntialiasedLineEnable = true;
	desc.RasterizerState.ForcedSampleCount = 0;
	desc.RasterizerState.ConservativeRaster = ConvertConservativeRasterizationModeToD3D12(_desc->mRasterizationState.mConservativeRasterizationMode);
	desc.DepthStencilState.DepthEnable = _desc->mDepthStencilState.mDepthWriteEnable;
	desc.DepthStencilState.DepthWriteMask = _desc->mDepthStencilState.mDepthWriteEnable ? D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.DepthStencilState.DepthFunc = ConvertComparisonFuncToD3D12(_desc->mDepthStencilState.mDepthCompareFunc);
	desc.DepthStencilState.StencilEnable = _desc->mDepthStencilState.mStencilTestEnable;
	desc.DepthStencilState.StencilReadMask = _desc->mDepthStencilState.mStencilReadMask;
	desc.DepthStencilState.StencilWriteMask = _desc->mDepthStencilState.mStencilWriteMask;
	desc.DepthStencilState.FrontFace.StencilFailOp = ConvertStencilOpToD3D12(_desc->mDepthStencilState.mFrontFaceDepthStencilOperator.mStencilFailOp);
	desc.DepthStencilState.FrontFace.StencilDepthFailOp = ConvertStencilOpToD3D12(_desc->mDepthStencilState.mFrontFaceDepthStencilOperator.mStencilDepthFailOp);
	desc.DepthStencilState.FrontFace.StencilPassOp = ConvertStencilOpToD3D12(_desc->mDepthStencilState.mFrontFaceDepthStencilOperator.mStencilPassOp);
	desc.DepthStencilState.FrontFace.StencilFunc = ConvertComparisonFuncToD3D12(_desc->mDepthStencilState.mFrontFaceDepthStencilOperator.mStencilCompareFunc);
	desc.DepthStencilState.BackFace.StencilFailOp = ConvertStencilOpToD3D12(_desc->mDepthStencilState.mBackFaceDepthStencilOperator.mStencilFailOp);
	desc.DepthStencilState.BackFace.StencilDepthFailOp = ConvertStencilOpToD3D12(_desc->mDepthStencilState.mBackFaceDepthStencilOperator.mStencilDepthFailOp);
	desc.DepthStencilState.BackFace.StencilPassOp = ConvertStencilOpToD3D12(_desc->mDepthStencilState.mBackFaceDepthStencilOperator.mStencilPassOp);
	desc.DepthStencilState.BackFace.StencilFunc = ConvertComparisonFuncToD3D12(_desc->mDepthStencilState.mBackFaceDepthStencilOperator.mStencilCompareFunc);
	desc.InputLayout = static_cast<SD3D12InputLayout*>(_desc->mInputLayout)->GetD3D12InputLayout();
	desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE::D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	desc.PrimitiveTopologyType = ConvertPrimitiveTopologyTypeToD3D12(_desc->mPrimitiveTopologyType);
	desc.NumRenderTargets = _desc->mRenderTargetCount;
	desc.RTVFormats[0] = ConvertPixelFormatToD3D(_desc->mRenderTargetFormat[0]);
	desc.RTVFormats[1] = ConvertPixelFormatToD3D(_desc->mRenderTargetFormat[1]);
	desc.RTVFormats[2] = ConvertPixelFormatToD3D(_desc->mRenderTargetFormat[2]);
	desc.RTVFormats[3] = ConvertPixelFormatToD3D(_desc->mRenderTargetFormat[3]);
	desc.RTVFormats[4] = ConvertPixelFormatToD3D(_desc->mRenderTargetFormat[4]);
	desc.RTVFormats[5] = ConvertPixelFormatToD3D(_desc->mRenderTargetFormat[5]);
	desc.RTVFormats[6] = ConvertPixelFormatToD3D(_desc->mRenderTargetFormat[6]);
	desc.RTVFormats[7] = ConvertPixelFormatToD3D(_desc->mRenderTargetFormat[7]);
	desc.DSVFormat = ConvertPixelFormatToD3D(_desc->mDepthStencilFormat);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.NodeMask = 0;
	desc.CachedPSO.pCachedBlob = _desc->mCachedPSO.GetBuffer();
	desc.CachedPSO.CachedBlobSizeInBytes = _desc->mCachedPSO.GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;

	void* pipelineState = nullptr;
	VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12CreateGraphicsPipelineState_D3D12Impl(&desc, GetNativePtr(), &pipelineState));
	return mGraphicsPipelineStatePool.AllocateElement(pipelineState);
}

IRDIComputePipelineState* SD3D12Device::CreateComputePipelineState(const SRDIComputePipelineState* _desc) noexcept
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
	desc.pRootSignature = static_cast<SD3D12RootSignature*>(_desc->mRootSignature)->GetNativePtr();
	desc.CS.pShaderBytecode = static_cast<SD3D12ComputeShader*>(_desc->mComputeShader)->GetCompiledShaderBlob().GetBuffer();
	desc.CS.BytecodeLength = static_cast<SD3D12ComputeShader*>(_desc->mComputeShader)->GetCompiledShaderBlob().GetBufferSize();
	desc.NodeMask = 0;
	desc.CachedPSO.pCachedBlob = _desc->mCachedPSO.GetBuffer();
	desc.CachedPSO.CachedBlobSizeInBytes = _desc->mCachedPSO.GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;

	void* pipelineState = nullptr;
	VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12CreateComputePipelineState_D3D12Impl(&desc, GetNativePtr(), &pipelineState));
	return mComputePipelineStatePool.AllocateElement(pipelineState);
}

IRDIBuffer* SD3D12Device::CreateBuffer(const SRDIBufferResourceDesc* _desc) noexcept
{
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = _desc->mBufferSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	void* nativeResourcePtr = CreateCommittedResource(_desc->mHeapType, &desc, ConvertResourceStateToD3D12(_desc->mResourceState));

	SD3D12Buffer* buffer = mBufferPool.AllocateElement();
	buffer->Init(nativeResourcePtr, _desc, this);
	return buffer;
}

IRDITexture1D* SD3D12Device::CreateTexture1D(const SRDITexture1DResourceDesc* _desc) noexcept
{
	CHECK(_desc->mSizeX <= D3D12_TEXTURE_RESOLUTION);
	CHECK(_desc->mMipCount <= D3D12_MIPMAPCOUNT);

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE1D;
	desc.Alignment = 0;
	desc.Width = _desc->mSizeX;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = _desc->mMipCount;
	desc.Format = ConvertPixelFormatToD3D(_desc->mPixelFormat);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	void* nativeResourcePtr = CreateCommittedResource(_desc->mHeapType, &desc, ConvertResourceStateToD3D12(_desc->mResourceState));

	SD3D12Texture1D* texture1D = mTexture1DPool.AllocateElement();
	texture1D->Init(nativeResourcePtr, _desc, this);
	return texture1D;
}

IRDITexture1DArray* SD3D12Device::CreateTexture1DArray(const SRDITexture1DArrayResourceDesc* _desc) noexcept
{
	CHECK(_desc->mSizeX <= D3D12_TEXTURE_RESOLUTION);
	CHECK(_desc->mMipCount <= D3D12_MIPMAPCOUNT);

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE1D;
	desc.Alignment = 0;
	desc.Width = _desc->mSizeX;
	desc.Height = 1;
	desc.DepthOrArraySize = _desc->mArraySize;
	desc.MipLevels = _desc->mMipCount;
	desc.Format = ConvertPixelFormatToD3D(_desc->mPixelFormat);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	void* nativeResourcePtr = CreateCommittedResource(_desc->mHeapType, &desc, ConvertResourceStateToD3D12(_desc->mResourceState));

	SD3D12Texture1DArray* texture1DArray = mTexture1DArrayPool.AllocateElement();
	texture1DArray->Init(nativeResourcePtr, _desc, this);
	return texture1DArray;
}

IRDITexture2D* SD3D12Device::CreateTexture2D(const SRDITexture2DResourceDesc* _desc) noexcept
{
	CHECK(_desc->mSizeX <= D3D12_TEXTURE_RESOLUTION);
	CHECK(_desc->mSizeY <= D3D12_TEXTURE_RESOLUTION);
	CHECK(_desc->mMipCount <= D3D12_MIPMAPCOUNT);

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = _desc->mSizeX;
	desc.Height = _desc->mSizeY;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = _desc->mMipCount;
	desc.Format = ConvertPixelFormatToD3D(_desc->mPixelFormat);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	void* nativeResourcePtr = CreateCommittedResource(_desc->mHeapType, &desc, ConvertResourceStateToD3D12(_desc->mResourceState));

	SD3D12Texture2D* texture2D = mTexture2DPool.AllocateElement();
	texture2D->Init(nativeResourcePtr, _desc, this);
	return texture2D;
}

IRDITexture2DArray* SD3D12Device::CreateTexture2DArray(const SRDITexture2DArrayResourceDesc* _desc) noexcept
{
	CHECK(_desc->mSizeX <= D3D12_TEXTURE_RESOLUTION);
	CHECK(_desc->mSizeY <= D3D12_TEXTURE_RESOLUTION);
	CHECK(_desc->mMipCount <= D3D12_MIPMAPCOUNT);

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = _desc->mSizeX;
	desc.Height = _desc->mSizeY;
	desc.DepthOrArraySize = _desc->mArraySize;
	desc.MipLevels = _desc->mMipCount;
	desc.Format = ConvertPixelFormatToD3D(_desc->mPixelFormat);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	void* nativeResourcePtr = CreateCommittedResource(_desc->mHeapType, &desc, ConvertResourceStateToD3D12(_desc->mResourceState));

	SD3D12Texture2DArray* texture2DArray = mTexture2DArrayPool.AllocateElement();
	texture2DArray->Init(nativeResourcePtr, _desc, this);
	return texture2DArray;
}

IRDITexture3D* SD3D12Device::CreateTexture3D(const SRDITexture3DResourceDesc* _desc) noexcept
{
	CHECK(_desc->mSizeX <= D3D12_TEXTURE_RESOLUTION);
	CHECK(_desc->mSizeY <= D3D12_TEXTURE_RESOLUTION);
	CHECK(_desc->mSizeZ <= D3D12_TEXTURE_RESOLUTION);
	CHECK(_desc->mMipCount <= D3D12_MIPMAPCOUNT);

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE3D;
	desc.Alignment = 0;
	desc.Width = _desc->mSizeX;
	desc.Height = _desc->mSizeY;
	desc.DepthOrArraySize = _desc->mSizeZ;
	desc.MipLevels = _desc->mMipCount;
	desc.Format = ConvertPixelFormatToD3D(_desc->mPixelFormat);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	void* nativeResourcePtr = CreateCommittedResource(_desc->mHeapType, &desc, ConvertResourceStateToD3D12(_desc->mResourceState));

	SD3D12Texture3D* texture3D = mTexture3DPool.AllocateElement();
	texture3D->Init(nativeResourcePtr, _desc, this);
	return texture3D;
}

IRDITextureCube* SD3D12Device::CreateTextureCube(const SRDITextureCubeResourceDesc* _desc) noexcept
{
	CHECK(_desc->mSizeXY <= D3D12_TEXTURE_RESOLUTION);
	CHECK(_desc->mMipCount <= D3D12_MIPMAPCOUNT);

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = _desc->mSizeXY;
	desc.Height = _desc->mSizeXY;
	desc.DepthOrArraySize = EnumToInt(ERDITextureCubeFace::FaceCount);
	desc.MipLevels = _desc->mMipCount;
	desc.Format = ConvertPixelFormatToD3D(_desc->mPixelFormat);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	void* nativeResourcePtr = CreateCommittedResource(_desc->mHeapType, &desc, ConvertResourceStateToD3D12(_desc->mResourceState));

	SD3D12TextureCube* textureCube = mTextureCubePool.AllocateElement();
	textureCube->Init(nativeResourcePtr, _desc, this);
	return textureCube;
}

IRDITextureCubeArray* SD3D12Device::CreateTextureCubeArray(const SRDITextureCubeArrayResourceDesc* _desc) noexcept
{
	CHECK(_desc->mSizeXY <= D3D12_TEXTURE_RESOLUTION);
	CHECK(_desc->mMipCount <= D3D12_MIPMAPCOUNT);

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = _desc->mSizeXY;
	desc.Height = _desc->mSizeXY;
	desc.DepthOrArraySize = _desc->mArraySize * EnumToInt(ERDITextureCubeFace::FaceCount);
	desc.MipLevels = _desc->mMipCount;
	desc.Format = ConvertPixelFormatToD3D(_desc->mPixelFormat);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	void* nativeResourcePtr = CreateCommittedResource(_desc->mHeapType, &desc, ConvertResourceStateToD3D12(_desc->mResourceState));

	SD3D12TextureCubeArray* textureCubeArray = mTextureCubeArrayPool.AllocateElement();
	textureCubeArray->Init(nativeResourcePtr, _desc, this);
	return textureCubeArray;
}

IRDISampler* SD3D12Device::CreateSampler(const SRDISamplerResourceDesc* _desc) noexcept
{
	SD3D12Sampler* sampler = mSamplerPool.AllocateElement();
	sampler->Init(_desc, this);
	return sampler;
}

IRDIDescriptorHeapRange* SD3D12Device::CreateDescriptorRange(uint16_t _srvCount, uint16_t _uavCount) noexcept
{
	return mShaderVisibleDescriptorHeap.AllocateDescriptorHeapRange(_srvCount, _uavCount);
}

IRDISamplerHeapRange* SD3D12Device::CreateDescriptorRange(uint16_t _samplerCount) noexcept
{
	return mShaderVisibleDescriptorHeap.AllocateSamplerHeapRange(_samplerCount);
}

IRDIVertexShader* SD3D12Device::CreateVertexShader(SBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept
{
	SBlob compiledShaderBlob;
	bool res = CreateShader(_hlslShader, ED3DShaderTarget::VS_5_0, _shaderMacro, &compiledShaderBlob, _shaderCompileError);

	if (!res)
		return nullptr;
	return mVertexShaderPool.AllocateElement(std::move(compiledShaderBlob));
}

IRDIHullShader* SD3D12Device::CreateHullShader(SBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept
{
	SBlob compiledShaderBlob;
	bool res = CreateShader(_hlslShader, ED3DShaderTarget::HS_5_0, _shaderMacro, &compiledShaderBlob, _shaderCompileError);

	if (!res)
		return nullptr;
	return mHullShaderPool.AllocateElement(std::move(compiledShaderBlob));
}

IRDIDomainShader* SD3D12Device::CreateDomainShader(SBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept
{
	SBlob compiledShaderBlob;
	bool res = CreateShader(_hlslShader, ED3DShaderTarget::DS_5_0, _shaderMacro, &compiledShaderBlob, _shaderCompileError);

	if (!res)
		return nullptr;
	return mDomainShaderPool.AllocateElement(std::move(compiledShaderBlob));
}

IRDIGeometryShader* SD3D12Device::CreateGeometryShader(SBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept
{
	SBlob compiledShaderBlob;
	bool res = CreateShader(_hlslShader, ED3DShaderTarget::GS_5_0, _shaderMacro, &compiledShaderBlob, _shaderCompileError);

	if (!res)
		return nullptr;
	else
		return mGeometryShaderPool.AllocateElement(std::move(compiledShaderBlob));
}

IRDIPixelShader* SD3D12Device::CreatePixelShader(SBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept
{
	SBlob compiledShaderBlob;
	bool res = CreateShader(_hlslShader, ED3DShaderTarget::PS_5_0, _shaderMacro, &compiledShaderBlob, _shaderCompileError);

	if (!res)
		return nullptr;
	else
		return mPixelShaderPool.AllocateElement(std::move(compiledShaderBlob));
}

IRDIComputeShader* SD3D12Device::CreateComputeShader(SBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept
{
	SBlob compiledShaderBlob;
	bool res = CreateShader(_hlslShader, ED3DShaderTarget::CS_5_0, _shaderMacro, &compiledShaderBlob, _shaderCompileError);

	if (!res)
		return nullptr;
	else
		return mComputeShaderPool.AllocateElement(std::move(compiledShaderBlob));
}

IRDIVertexShader* SD3D12Device::CreateVertexShader(SBufferView _compiledShader) noexcept
{
	return mVertexShaderPool.AllocateElement(_compiledShader.GetBuffer(), _compiledShader.GetBufferSize());
}

IRDIHullShader* SD3D12Device::CreateHullShader(SBufferView _compiledShader) noexcept
{
	return mHullShaderPool.AllocateElement(_compiledShader.GetBuffer(), _compiledShader.GetBufferSize());
}

IRDIDomainShader* SD3D12Device::CreateDomainShader(SBufferView _compiledShader) noexcept
{
	return mDomainShaderPool.AllocateElement(_compiledShader.GetBuffer(), _compiledShader.GetBufferSize());
}

IRDIGeometryShader* SD3D12Device::CreateGeometryShader(SBufferView _compiledShader) noexcept
{
	return mGeometryShaderPool.AllocateElement(_compiledShader.GetBuffer(), _compiledShader.GetBufferSize());
}

IRDIPixelShader* SD3D12Device::CreatePixelShader(SBufferView _compiledShader) noexcept
{
	return mPixelShaderPool.AllocateElement(_compiledShader.GetBuffer(), _compiledShader.GetBufferSize());
}

IRDIComputeShader* SD3D12Device::CreateComputeShader(SBufferView _compiledShader) noexcept
{
	return mComputeShaderPool.AllocateElement(_compiledShader.GetBuffer(), _compiledShader.GetBufferSize());
}

IRDITexture2D* SD3D12Device::CreateTexture2DWithCreatedResource(const SRDITexture2DResourceDesc* _desc, void* _resource) noexcept
{
	SD3D12Texture2D* texture2D = mTexture2DPool.AllocateElement();
	texture2D->Init(_resource, _desc, this);
	return texture2D;
}

void* SD3D12Device::CreateCommittedResource(ERDIHeapType _heapType, const D3D12_RESOURCE_DESC* _desc, D3D12_RESOURCE_STATES _state) noexcept
{
	D3D12_HEAP_PROPERTIES heapProperties = {};
	heapProperties.Type = ConvertHeapTypeToD3D12(_heapType);
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;

	void* nativeResourcePtr = nullptr;
	VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12CreateCommittedResource_D3D12Impl(GetNativePtr(), &heapProperties, static_cast<int32_t>(D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE), _desc, static_cast<int32_t>(_state), nullptr, &nativeResourcePtr));
	return nativeResourcePtr;
}

bool SD3D12Device::CreateShader(SBufferView _hlslShader, ED3DShaderTarget _shaderTarget, const SRDIShaderMacro* _shaderMacro, SBlob* _shaderBlob, SRDIErrorInfo* _shaderCompileError) noexcept
{
	static D3D_SHADER_MACRO shaderMacro[64] = {};
	static char macroBuffer[64][128] = {};

	for (size_t i = 0; i != _shaderMacro->mDefinedMacro.size(); ++i)
	{
		std::wstring_view currentMacro = _shaderMacro->mDefinedMacro[i];
		std::string macro = Locale::ConvertWstringToString(Locale::ECodePage::ACP, currentMacro);
		strcpy_s(macroBuffer[i], 128, macro.c_str());

		shaderMacro[i].Name = macroBuffer[i];
		shaderMacro[i].Definition = nullptr;
	}

	shaderMacro[_shaderMacro->mDefinedMacro.size()].Name = nullptr;
	shaderMacro[_shaderMacro->mDefinedMacro.size()].Definition = nullptr;

	std::vector<uint8_t> shaderBlob;
	std::vector<uint8_t> errorBlob;
	uint32_t res = D3DAPIWarp_Impl::D3DCompile_D3DImpl(_hlslShader.GetBuffer(), _hlslShader.GetBufferSize(), nullptr, &shaderMacro, mSearchShaderHeaderFileFunc, "main", ConvertShaderTargetToStr(_shaderTarget), static_cast<int32_t>(mShaderCompileFlag), shaderBlob, errorBlob);

	if (res == 0)
		_shaderBlob->ResizeBlob(shaderBlob.data(), shaderBlob.size());
	else
		GenerateErrorInfo(errorBlob, _shaderCompileError);

	return res == 0;
}

void SD3D12Device::GenerateErrorInfo(const std::vector<uint8_t>& _errorBlob, SRDIErrorInfo* _errorInfo)
{
	if (_errorInfo == nullptr)
		return;

	if (_errorBlob.empty())
	{
		_errorInfo->mParsedErrorString.clear();
		_errorInfo->mErrorString.clear();
		return;
	}

	std::string compiledError = reinterpret_cast<const char*>(_errorBlob.data());
	_errorInfo->mErrorString = Locale::ConvertStringToWstring(Locale::ECodePage::ACP, compiledError);
	const wchar_t* errorStr = _errorInfo->mErrorString.c_str();

	size_t begin = 0;
	while (true)
	{
		size_t end = _errorInfo->mErrorString.find_first_of(L'\n', begin);
		if (end == std::wstring::npos)
			break;

		_errorInfo->mParsedErrorString.push_back(std::wstring_view(errorStr + begin, errorStr + end));
		begin = end + 1;
	}
}

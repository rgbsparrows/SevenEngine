#include "D3D/D3DUtil.h"
#include "Core/Misc/Localize.h"
#include "D3D/D3D12/D3D12Device.h"
#include "D3D/D3D12/D3D12Factory.h"
#include "D3D/Helper/D3DEnumConvertor.h"
#include "Core/ProgramConfiguation/BasicPath.h"
#include "D3D/D3D12/Helper/D3D12EnumConvertor.h"

void SD3D12Device::Init(ID3D12Device* _nativePtr, SD3D12Adapter* _adapter, SD3D12Factory* _factory) noexcept
{
	{
		mD3D12DeviceNativePtr = _nativePtr;
		mAdapter = _adapter;
		mFactory = _factory;
	}

	//CommandQuue
	{
		ID3D12CommandQueue* commandQueueNativePtr = nullptr;

		D3D12_COMMAND_QUEUE_DESC queueDesc;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Priority = 0;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.NodeMask = 0;
		VERIFY_D3D_RETURN(GetNativePtr()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueueNativePtr)));

		ID3D12Fence* fenceNativePtr = nullptr;
		VERIFY_D3D_RETURN(GetNativePtr()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fenceNativePtr)));
		mCommandQueue.Init(commandQueueNativePtr, fenceNativePtr);
	}

	//ComputeCommandQuue
	{
		ID3D12CommandQueue* computeCommandQueueNativePtr = nullptr;

		D3D12_COMMAND_QUEUE_DESC queueDesc;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
		queueDesc.Priority = 0;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.NodeMask = 0;
		VERIFY_D3D_RETURN(GetNativePtr()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&computeCommandQueueNativePtr)));

		ID3D12Fence* fenceNativePtr = nullptr;
		VERIFY_D3D_RETURN(GetNativePtr()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fenceNativePtr)));
		mComputeCommandQueue.Init(computeCommandQueueNativePtr, fenceNativePtr);
	}

	//DescriptorHeaps
	{
		ID3D12DescriptorHeap* shaderVisibleCbvSrvUavDescriptorHeap = nullptr;
		ID3D12DescriptorHeap* shaderVisibleSamplerViewDescriptorHeap = nullptr;
		ID3D12DescriptorHeap* rtvDescriptorHeap = nullptr;
		ID3D12DescriptorHeap* dsvDescriptorHeap = nullptr;
		ID3D12DescriptorHeap* srvDescriptorHeap = nullptr;
		ID3D12DescriptorHeap* uavDescriptorHeap = nullptr;
		ID3D12DescriptorHeap* samplerViewDescriptorHeap = nullptr;

		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.NodeMask = 0;

		{
			heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

			heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			heapDesc.NumDescriptors = ShaderVisibleCbvSrvUavDescriptorCount;
			VERIFY_D3D_RETURN(GetNativePtr()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&shaderVisibleCbvSrvUavDescriptorHeap)));

			heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
			heapDesc.NumDescriptors = ShaderVisibleSamplerViewDescriptorCount;
			VERIFY_D3D_RETURN(GetNativePtr()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&shaderVisibleSamplerViewDescriptorHeap)));
		}

		{
			heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

			heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			heapDesc.NumDescriptors = RTVDescriptorCount;
			VERIFY_D3D_RETURN(GetNativePtr()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvDescriptorHeap)));

			heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			heapDesc.NumDescriptors = DSVDescriptorCount;
			VERIFY_D3D_RETURN(GetNativePtr()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&dsvDescriptorHeap)));

			heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			heapDesc.NumDescriptors = SRVDescriptorCount;
			VERIFY_D3D_RETURN(GetNativePtr()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&srvDescriptorHeap)));

			heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			heapDesc.NumDescriptors = UAVDescriptorCount;
			VERIFY_D3D_RETURN(GetNativePtr()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&uavDescriptorHeap)));

			heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			heapDesc.NumDescriptors = SamplerViewDescriptorCount;
			VERIFY_D3D_RETURN(GetNativePtr()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&samplerViewDescriptorHeap)));
		}

		mShaderVisibleDescriptorHeap.Init(shaderVisibleCbvSrvUavDescriptorHeap, ShaderVisibleCbvSrvUavDescriptorCount, shaderVisibleSamplerViewDescriptorHeap, ShaderVisibleSamplerViewDescriptorCount, this);
		mDescriptorHeap.Init(rtvDescriptorHeap, RTVDescriptorCount, dsvDescriptorHeap, DSVDescriptorCount, srvDescriptorHeap, SRVDescriptorCount, uavDescriptorHeap, UAVDescriptorCount, samplerViewDescriptorHeap, SamplerViewDescriptorCount, this);
	}

	//DescriptorHandleIncrementSize
	{
		for (int32_t i = 0; i != EnumToInt(D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES); ++i)
		{
			mDescriptorHandleIncrement[i] = GetNativePtr()->GetDescriptorHandleIncrementSize(static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(i));
		}
	}
}

void SD3D12Device::Clear() noexcept
{
	TODO("SD3D12Device Clear");
}

IRDICommandAllocator* SD3D12Device::CreateCommandAllocator(ERDICommandListType _commandListType) noexcept
{
	ID3D12CommandAllocator* commandAllocatorNativePtr = nullptr;
	GetNativePtr()->CreateCommandAllocator(ConvertCommandListTypeToD3D12(_commandListType), IID_PPV_ARGS(&commandAllocatorNativePtr));

	SD3D12CommandAllocator* commandAllcoator = mCommandAllocatorPool.AllocateElement();
	commandAllcoator->Init(commandAllocatorNativePtr, this);

	return commandAllcoator;
}

IRDICommandList* SD3D12Device::CreateCommandList(ERDICommandListType _commandListType, IRDICommandAllocator* _commandAllocator) noexcept
{
	SD3D12CommandAllocator* commandAllocator = static_cast<SD3D12CommandAllocator*>(_commandAllocator);

	ID3D12GraphicsCommandList* commandListNativePtr = nullptr; 
	GetNativePtr()->CreateCommandList(0, ConvertCommandListTypeToD3D12(_commandListType), commandAllocator->GetNativePtr(), nullptr, IID_PPV_ARGS(&commandListNativePtr));

	SD3D12CommandList* commandList = mCommandListPool.AllocateElement();
	commandList->Init(commandListNativePtr, this);
	commandList->Close();

	return commandList;
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
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = _swapChainDesc->mBufferCount;
	desc.OutputWindow = _swapChainDesc->mOutputWindow;
	desc.Windowed = _swapChainDesc->mIsWindowed;
	desc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Flags = 0;

	IDXGISwapChain* swapchainNativePtr = nullptr;
	VERIFY_D3D_RETURN(mFactory->GetNativePtr()->CreateSwapChain(mCommandQueue.GetCommandQueueNativePtr(), &desc, &swapchainNativePtr));

	SD3D12SwapChain* swapChain = mSwapChainPool.AllocateElement();
	swapChain->Init(swapchainNativePtr, _swapChainDesc, this, mAdapter);
	return swapChain;
}

IRDIGraphicsPipelineState* SD3D12Device::CreateGraphicsPipelineState(const SRDIGraphicsPipelineStateDesc* _desc) noexcept
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};

	//RootSignature
	{
		SBlob rootSignatureBlob = CreateRootSignatureBlob(&_desc->mRootSignature);
		ID3D12RootSignature* rootSignatureNativePtr = nullptr;
		VERIFY_D3D_RETURN(GetNativePtr()->CreateRootSignature(0, rootSignatureBlob.GetBuffer(), rootSignatureBlob.GetBufferSize(), IID_PPV_ARGS(&rootSignatureNativePtr)));

		desc.pRootSignature = rootSignatureNativePtr;
	}

	//Shader
	{
		desc.VS.pShaderBytecode = _desc->mVertexShader.GetBuffer();
		desc.VS.BytecodeLength = _desc->mVertexShader.GetBufferSize();
		desc.HS.pShaderBytecode = _desc->mHullShader.GetBuffer();
		desc.HS.BytecodeLength = _desc->mHullShader.GetBufferSize();
		desc.DS.pShaderBytecode = _desc->mDomainShader.GetBuffer();
		desc.DS.BytecodeLength = _desc->mDomainShader.GetBufferSize();
		desc.GS.pShaderBytecode = _desc->mGeometryShader.GetBuffer();
		desc.GS.BytecodeLength = _desc->mGeometryShader.GetBufferSize();
		desc.PS.pShaderBytecode = _desc->mPixelShader.GetBuffer();
		desc.PS.BytecodeLength = _desc->mPixelShader.GetBufferSize();
	}

	//Stream Output
	{
		desc.StreamOutput.pSODeclaration = nullptr;
		desc.StreamOutput.NumEntries = 0;
		desc.StreamOutput.pBufferStrides = nullptr;
		desc.StreamOutput.NumStrides = 0;
		desc.StreamOutput.RasterizedStream = 0;
	}

	// BlendState
	{
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
	}

	desc.SampleMask = UINT_MAX;

	//RasterizerState
	{
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
	}

	//DepthStencilState
	{
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
	}

	//InputLayout
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElements;
	{
		for (auto& _ele : _desc->mInputLayout.mInputElements)
		{
			D3D12_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = _ele.mSemanticName.c_str();
			elementDesc.SemanticIndex = 0;
			elementDesc.Format = ConvertPixelFormatToD3D(_ele.mFormat);
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = _ele.mAlignedByteOffset;
			elementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			inputElements.push_back(elementDesc);
		}

		desc.InputLayout.pInputElementDescs = inputElements.data();
		desc.InputLayout.NumElements = static_cast<UINT>(inputElements.size());
	}


	desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE::D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	desc.PrimitiveTopologyType = ConvertPrimitiveTopologyTypeToD3D12(_desc->mPrimitiveTopologyType);

	//RTFormat
	{
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
	}

	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.NodeMask = 0;
	desc.CachedPSO.pCachedBlob = _desc->mCachedPSO.GetBuffer();
	desc.CachedPSO.CachedBlobSizeInBytes = _desc->mCachedPSO.GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;

	ID3D12PipelineState* nativePipelineState = nullptr;
	VERIFY_D3D_RETURN(GetNativePtr()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&nativePipelineState)));

	SD3D12GraphicsPipelineState* pipelineState = mGraphicsPipelineStatePool.AllocateElement();
	pipelineState->Init(nativePipelineState, desc.pRootSignature, this);
	return pipelineState;
}

IRDIComputePipelineState* SD3D12Device::CreateComputePipelineState(const SRDIComputePipelineStateDesc* _desc) noexcept
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};

	//RootSignature
	{
		SBlob rootSignatureBlob = CreateRootSignatureBlob(&_desc->mRootSignature);
		ID3D12RootSignature* rootSignatureNativePtr = nullptr;
		VERIFY_D3D_RETURN(GetNativePtr()->CreateRootSignature(0, rootSignatureBlob.GetBuffer(), rootSignatureBlob.GetBufferSize(), IID_PPV_ARGS(&rootSignatureNativePtr)));

		desc.pRootSignature = rootSignatureNativePtr;
	}

	//Shader
	{
		desc.CS.pShaderBytecode = _desc->mComputeShader.GetBuffer();
		desc.CS.BytecodeLength = _desc->mComputeShader.GetBufferSize();
	}

	desc.NodeMask = 0;
	desc.CachedPSO.pCachedBlob = _desc->mCachedPSO.GetBuffer();
	desc.CachedPSO.CachedBlobSizeInBytes = _desc->mCachedPSO.GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;

	ID3D12PipelineState* nativePipelineState = nullptr;
	VERIFY_D3D_RETURN(GetNativePtr()->CreateComputePipelineState(&desc, IID_PPV_ARGS(&nativePipelineState)));

	SD3D12ComputePipelineState* pipelineState = mComputePipelineStatePool.AllocateElement();
	pipelineState->Init(nativePipelineState, desc.pRootSignature, this);
	return pipelineState;
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
	desc.Flags = MakeD3D12ResourceFlag(_desc->mResourceUsage);

	ID3D12Resource* nativeResourcePtr = CreateCommittedResource(_desc->mHeapType, &desc, nullptr);

	SD3D12Buffer* buffer = mBufferPool.AllocateElement();
	buffer->Init(nativeResourcePtr, _desc, this);
	return buffer;
}

IRDITexture1D* SD3D12Device::CreateTexture1D(const SRDITexture1DResourceDesc* _desc) noexcept
{
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
	desc.Flags = MakeD3D12ResourceFlag(_desc->mResourceUsage);
	
	D3D12_CLEAR_VALUE clearValue = MakeD3D12ClearValue(_desc->mResourceUsage, _desc->mPixelFormat, _desc->mRtvPixelFormat, _desc->mDsvPixelFormat, _desc->mClearColor, _desc->mClearDepth, _desc->mClearStencil);
	const D3D12_CLEAR_VALUE* pClearValue = ((_desc->mResourceUsage & (ERDIResourceUsage::RenderTarget | ERDIResourceUsage::DepthStencil)) != ERDIResourceUsage::None) ? &clearValue : nullptr;

	ID3D12Resource* nativeResourcePtr = CreateCommittedResource(_desc->mHeapType, &desc, pClearValue);

	SD3D12Texture1D* texture1D = mTexture1DPool.AllocateElement();
	texture1D->Init(nativeResourcePtr, _desc, this);
	return texture1D;
}

IRDITexture1DArray* SD3D12Device::CreateTexture1DArray(const SRDITexture1DArrayResourceDesc* _desc) noexcept
{
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
	desc.Flags = MakeD3D12ResourceFlag(_desc->mResourceUsage);

	D3D12_CLEAR_VALUE clearValue = MakeD3D12ClearValue(_desc->mResourceUsage, _desc->mPixelFormat, _desc->mRtvPixelFormat, _desc->mDsvPixelFormat, _desc->mClearColor, _desc->mClearDepth, _desc->mClearStencil);
	const D3D12_CLEAR_VALUE* pClearValue = ((_desc->mResourceUsage & (ERDIResourceUsage::RenderTarget | ERDIResourceUsage::DepthStencil)) != ERDIResourceUsage::None) ? &clearValue : nullptr;

	ID3D12Resource* nativeResourcePtr = CreateCommittedResource(_desc->mHeapType, &desc, pClearValue);

	SD3D12Texture1DArray* texture1DArray = mTexture1DArrayPool.AllocateElement();
	texture1DArray->Init(nativeResourcePtr, _desc, this);
	return texture1DArray;
}

IRDITexture2D* SD3D12Device::CreateTexture2D(const SRDITexture2DResourceDesc* _desc) noexcept
{
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
	desc.Flags = MakeD3D12ResourceFlag(_desc->mResourceUsage);

	D3D12_CLEAR_VALUE clearValue = MakeD3D12ClearValue(_desc->mResourceUsage, _desc->mPixelFormat, _desc->mRtvPixelFormat, _desc->mDsvPixelFormat, _desc->mClearColor, _desc->mClearDepth, _desc->mClearStencil);
	const D3D12_CLEAR_VALUE* pClearValue = ((_desc->mResourceUsage & (ERDIResourceUsage::RenderTarget | ERDIResourceUsage::DepthStencil)) != ERDIResourceUsage::None) ? &clearValue : nullptr;

	ID3D12Resource* nativeResourcePtr = CreateCommittedResource(_desc->mHeapType, &desc, pClearValue);

	SD3D12Texture2D* texture2D = mTexture2DPool.AllocateElement();
	texture2D->Init(nativeResourcePtr, _desc, this);
	return texture2D;
}

IRDITexture2DArray* SD3D12Device::CreateTexture2DArray(const SRDITexture2DArrayResourceDesc* _desc) noexcept
{
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
	desc.Flags = MakeD3D12ResourceFlag(_desc->mResourceUsage);

	D3D12_CLEAR_VALUE clearValue = MakeD3D12ClearValue(_desc->mResourceUsage, _desc->mPixelFormat, _desc->mRtvPixelFormat, _desc->mDsvPixelFormat, _desc->mClearColor, _desc->mClearDepth, _desc->mClearStencil);
	const D3D12_CLEAR_VALUE* pClearValue = ((_desc->mResourceUsage & (ERDIResourceUsage::RenderTarget | ERDIResourceUsage::DepthStencil)) != ERDIResourceUsage::None) ? &clearValue : nullptr;

	ID3D12Resource* nativeResourcePtr = CreateCommittedResource(_desc->mHeapType, &desc, pClearValue);

	SD3D12Texture2DArray* texture2DArray = mTexture2DArrayPool.AllocateElement();
	texture2DArray->Init(nativeResourcePtr, _desc, this);
	return texture2DArray;
}

IRDITexture3D* SD3D12Device::CreateTexture3D(const SRDITexture3DResourceDesc* _desc) noexcept
{
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
	desc.Flags = MakeD3D12ResourceFlag(_desc->mResourceUsage);

	D3D12_CLEAR_VALUE clearValue = MakeD3D12ClearValue(_desc->mResourceUsage, _desc->mPixelFormat, _desc->mRtvPixelFormat, _desc->mDsvPixelFormat, _desc->mClearColor, _desc->mClearDepth, _desc->mClearStencil);
	const D3D12_CLEAR_VALUE* pClearValue = ((_desc->mResourceUsage & (ERDIResourceUsage::RenderTarget | ERDIResourceUsage::DepthStencil)) != ERDIResourceUsage::None) ? &clearValue : nullptr;

	ID3D12Resource* nativeResourcePtr = CreateCommittedResource(_desc->mHeapType, &desc, pClearValue);

	SD3D12Texture3D* texture3D = mTexture3DPool.AllocateElement();
	texture3D->Init(nativeResourcePtr, _desc, this);
	return texture3D;
}

IRDITextureCube* SD3D12Device::CreateTextureCube(const SRDITextureCubeResourceDesc* _desc) noexcept
{
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
	desc.Flags = MakeD3D12ResourceFlag(_desc->mResourceUsage);

	D3D12_CLEAR_VALUE clearValue = MakeD3D12ClearValue(_desc->mResourceUsage, _desc->mPixelFormat, _desc->mRtvPixelFormat, _desc->mDsvPixelFormat, _desc->mClearColor, _desc->mClearDepth, _desc->mClearStencil);
	const D3D12_CLEAR_VALUE* pClearValue = ((_desc->mResourceUsage & (ERDIResourceUsage::RenderTarget | ERDIResourceUsage::DepthStencil)) != ERDIResourceUsage::None) ? &clearValue : nullptr;

	ID3D12Resource* nativeResourcePtr = CreateCommittedResource(_desc->mHeapType, &desc, pClearValue);

	SD3D12TextureCube* textureCube = mTextureCubePool.AllocateElement();
	textureCube->Init(nativeResourcePtr, _desc, this);
	return textureCube;
}

IRDITextureCubeArray* SD3D12Device::CreateTextureCubeArray(const SRDITextureCubeArrayResourceDesc* _desc) noexcept
{
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
	desc.Flags = MakeD3D12ResourceFlag(_desc->mResourceUsage);

	D3D12_CLEAR_VALUE clearValue = MakeD3D12ClearValue(_desc->mResourceUsage, _desc->mPixelFormat, _desc->mRtvPixelFormat, _desc->mDsvPixelFormat, _desc->mClearColor, _desc->mClearDepth, _desc->mClearStencil);
	const D3D12_CLEAR_VALUE* pClearValue = ((_desc->mResourceUsage & (ERDIResourceUsage::RenderTarget | ERDIResourceUsage::DepthStencil)) != ERDIResourceUsage::None) ? &clearValue : nullptr;

	ID3D12Resource* nativeResourcePtr = CreateCommittedResource(_desc->mHeapType, &desc, pClearValue);

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

void SD3D12Device::ReleaseCommandAllocator(SD3D12CommandAllocator* _commandAllocator) noexcept
{
	_commandAllocator->GetNativePtr()->Release();
	mCommandAllocatorPool.DeallocateElement(_commandAllocator);
}

void SD3D12Device::ReleaseCommandList(SD3D12CommandList* _commandList) noexcept
{
	_commandList->GetNativePtr()->Release();
	mCommandListPool.DeallocateElement(_commandList);
}

void SD3D12Device::ReleaseSwapChain(SD3D12SwapChain* _swapChain) noexcept
{
	_swapChain->GetNativePtr()->Release();
	mSwapChainPool.DeallocateElement(_swapChain);
}

void SD3D12Device::ReleaseGraphicsPipelineState(SD3D12GraphicsPipelineState* _graphicPipelineState) noexcept
{
	_graphicPipelineState->GetNativePtr()->Release();
	_graphicPipelineState->GetRootSignatureNativePtr()->Release();
	mGraphicsPipelineStatePool.DeallocateElement(_graphicPipelineState);
}

void SD3D12Device::ReleaseComputePipelineState(SD3D12ComputePipelineState* _computePipelineState) noexcept
{
	_computePipelineState->GetNativePtr()->Release();
	_computePipelineState->GetRootSignatureNativePtr()->Release();
	mComputePipelineStatePool.DeallocateElement(_computePipelineState);
}

void SD3D12Device::ReleaseBuffer(SD3D12Buffer* _buffer) noexcept
{
	_buffer->GetNativePtr()->Release();
	mBufferPool.DeallocateElement(_buffer);
}

void SD3D12Device::ReleaseTexture1D(SD3D12Texture1D* _texture1D) noexcept
{
	_texture1D->GetNativePtr()->Release();
	mTexture1DPool.DeallocateElement(_texture1D);
}

void SD3D12Device::ReleaseTexture1DArray(SD3D12Texture1DArray* _texture1DArray) noexcept
{
	_texture1DArray->GetNativePtr()->Release();
	mTexture1DArrayPool.DeallocateElement(_texture1DArray);
}

void SD3D12Device::ReleaseTexture2D(SD3D12Texture2D* _texture2D) noexcept
{
	_texture2D->GetNativePtr()->Release();
	mTexture2DPool.DeallocateElement(_texture2D);
}

void SD3D12Device::ReleaseTexture2DArray(SD3D12Texture2DArray* _texture2DArray) noexcept
{
	_texture2DArray->GetNativePtr()->Release();
	mTexture2DArrayPool.DeallocateElement(_texture2DArray);
}

void SD3D12Device::ReleaseTexture3D(SD3D12Texture3D* _texture3D) noexcept
{
	_texture3D->GetNativePtr()->Release();
	mTexture3DPool.DeallocateElement(_texture3D);
}

void SD3D12Device::ReleaseTextureCube(SD3D12TextureCube* _textureCube) noexcept
{
	_textureCube->GetNativePtr()->Release();
	mTextureCubePool.DeallocateElement(_textureCube);
}

void SD3D12Device::ReleaseTextureCubeArray(SD3D12TextureCubeArray* _textureCubeArray) noexcept
{
	_textureCubeArray->GetNativePtr()->Release();
	mTextureCubeArrayPool.DeallocateElement(_textureCubeArray);
}

void SD3D12Device::ReleaseSampler(SD3D12Sampler* _sampler) noexcept
{
	mSamplerPool.DeallocateElement(_sampler);
}

IRDITexture2D* SD3D12Device::CreateTexture2DWithCreatedResource(const SRDITexture2DResourceDesc* _desc, ID3D12Resource* _resource) noexcept
{
	SD3D12Texture2D* texture2D = mTexture2DPool.AllocateElement();
	texture2D->Init(_resource, _desc, this);
	return texture2D;
}

D3D12_RESOURCE_FLAGS SD3D12Device::MakeD3D12ResourceFlag(ERDIResourceUsage _resourceUsage) noexcept
{
	D3D12_RESOURCE_FLAGS resourceFlag = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	if ((_resourceUsage & ERDIResourceUsage::RenderTarget) == ERDIResourceUsage::RenderTarget)
		resourceFlag |= D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	if ((_resourceUsage & ERDIResourceUsage::DepthStencil) == ERDIResourceUsage::DepthStencil)
		resourceFlag |= D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	if ((_resourceUsage & ERDIResourceUsage::UnorderAccess) == ERDIResourceUsage::UnorderAccess)
		resourceFlag |= D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	return resourceFlag;
}

D3D12_CLEAR_VALUE SD3D12Device::MakeD3D12ClearValue(ERDIResourceUsage _resourceUsage, ERDIPixelFormat _pixelFormat, ERDIPixelFormat _rtvPixelFormat, ERDIPixelFormat _dsvPixelFormat, Math::SFColor _clearColor, float _clearDepth, uint8_t _clearStencil) noexcept
{
	D3D12_CLEAR_VALUE clearValue = {};

	_rtvPixelFormat = _rtvPixelFormat == ERDIPixelFormat::UNKNOWN ? _pixelFormat : _rtvPixelFormat;
	_dsvPixelFormat = _dsvPixelFormat == ERDIPixelFormat::UNKNOWN ? _pixelFormat : _dsvPixelFormat;

	if ((_resourceUsage & ERDIResourceUsage::RenderTarget) == ERDIResourceUsage::RenderTarget)
	{
		clearValue.Format = ConvertPixelFormatToD3D(_rtvPixelFormat);
		clearValue.Color[0] = _clearColor[0];
		clearValue.Color[1] = _clearColor[1];
		clearValue.Color[2] = _clearColor[2];
		clearValue.Color[3] = _clearColor[3];
	}
	else if ((_resourceUsage & ERDIResourceUsage::DepthStencil) == ERDIResourceUsage::DepthStencil)
	{
		clearValue.Format = ConvertPixelFormatToD3D(_dsvPixelFormat);
		clearValue.DepthStencil.Depth = _clearDepth;
		clearValue.DepthStencil.Stencil = _clearStencil;
	}

	return clearValue;
}

ID3D12Resource* SD3D12Device::CreateCommittedResource(ERDIHeapType _heapType, const D3D12_RESOURCE_DESC* _desc, const D3D12_CLEAR_VALUE* _clearValue) noexcept
{
	if (_desc->Dimension != D3D12_RESOURCE_DIMENSION_BUFFER)
	{
		CHECK(_desc->Width <= D3D12_TEXTURE_RESOLUTION);
		CHECK(_desc->Height <= D3D12_TEXTURE_RESOLUTION);
		CHECK(_desc->DepthOrArraySize <= D3D12_TEXTURE_RESOLUTION);
		CHECK(_desc->MipLevels <= D3D12_MIPMAPCOUNT);
	}

	static constexpr D3D12_RESOURCE_STATES resourceStateMap[EnumToInt(ERDIHeapType::Num)] =
	{
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_COMMON,
	};

	D3D12_HEAP_PROPERTIES heapProperties = {};
	heapProperties.Type = ConvertHeapTypeToD3D12(_heapType);
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;

	ID3D12Resource* nativeResourcePtr = nullptr;
	VERIFY_D3D_RETURN(GetNativePtr()->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, _desc, resourceStateMap[EnumToInt(_heapType)], _clearValue, IID_PPV_ARGS(&nativeResourcePtr)));

	return nativeResourcePtr;
}

SBlob SD3D12Device::CreateRootSignatureBlob(const SRDIRootSignature* _rootSignature) noexcept
{
	static D3D12_DESCRIPTOR_RANGE descriptorRange[30] = {};
	static D3D12_ROOT_PARAMETER rootParameters[10] = {};
	static D3D12_STATIC_SAMPLER_DESC staticSamplerDesc[30] = {};

	D3D12_ROOT_SIGNATURE_DESC desc = {};

	for (size_t i = 0, j = 0; i != _rootSignature->mRootParameters.size(); ++i)
	{
		auto& _ele = _rootSignature->mRootParameters[i];
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

	for (size_t i = 0; i != _rootSignature->mStaticSamplerDescs.size(); ++i)
	{
		auto& _ele = _rootSignature->mStaticSamplerDescs[i];

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

	desc.NumParameters = static_cast<uint32_t>(_rootSignature->mRootParameters.size());
	desc.pParameters = rootParameters;
	desc.NumStaticSamplers = static_cast<uint32_t>(_rootSignature->mStaticSamplerDescs.size());
	desc.pStaticSamplers = staticSamplerDesc;
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* serlizedD3DBlob = nullptr;

	HRESULT res = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &serlizedD3DBlob, nullptr);

	SBlob serializedBlob(serlizedD3DBlob->GetBufferPointer(), serlizedD3DBlob->GetBufferSize());

	if (serlizedD3DBlob != nullptr)
		serlizedD3DBlob->Release();

	return serializedBlob;
}

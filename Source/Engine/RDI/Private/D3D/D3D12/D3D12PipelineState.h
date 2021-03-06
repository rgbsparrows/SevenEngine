#pragma once

#include "D3D/D3DUtil.h"
#include "Core/Container/Blob.h"
#include "RDI/Interface/RDIPipelineState.h"

class SD3D12PipelineStateBase
{
public:
	SD3D12PipelineStateBase(ID3D12PipelineState* _nativePtr) noexcept
	{
		mPipelineStateNativePtr = _nativePtr;

		ID3DBlob* cachedBlob = nullptr;
		mPipelineStateNativePtr->GetCachedBlob(&cachedBlob);

		mCachedBlob.ResizeBlob(cachedBlob->GetBufferPointer(), cachedBlob->GetBufferSize());
		if (cachedBlob != nullptr)
			cachedBlob->Release();
	}

	SBlob mCachedBlob;
	ID3D12PipelineState* mPipelineStateNativePtr;
};

class SD3D12GraphicsPipelineState : public IRDIGraphicsPipelineState, public SD3D12PipelineStateBase
{
public:
	SD3D12GraphicsPipelineState(ID3D12PipelineState* _nativePtr) noexcept
		:SD3D12PipelineStateBase(_nativePtr)
	{
	}

	ID3D12PipelineState* GetNativePtr() noexcept { return mPipelineStateNativePtr; }

public:
	SBufferView GetCachedBlob() noexcept override { return SBufferView(mCachedBlob); }
};


class SD3D12ComputePipelineState : public IRDIComputePipelineState, public SD3D12PipelineStateBase
{
public:
	SD3D12ComputePipelineState(ID3D12PipelineState* _nativePtr) noexcept
		:SD3D12PipelineStateBase(_nativePtr)
	{
	}

	ID3D12PipelineState* GetNativePtr() noexcept { return mPipelineStateNativePtr; }

public:
	SBufferView GetCachedBlob() noexcept override { return SBufferView(mCachedBlob); }
};

#pragma once

#include "D3D/D3DUtil.h"
#include "Core/Container/Blob.h"
#include "RDI/Interface/RDIPipelineState.h"
#include "D3D/D3D12/Warper/D3D12ImplWarper.h"

class SD3D12PipelineStateBase
{
public:
	SD3D12PipelineStateBase(void* _nativePtr) noexcept
	{
		mPipelineStateNativePtr = _nativePtr;

		std::vector<uint8_t> cachedBlob;
		VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12GetPipelineStateCachedBlob(_nativePtr, cachedBlob));
		mCachedBlob.ResizeBlob(cachedBlob.data(), cachedBlob.size());
	}

	SBlob mCachedBlob;
	void* mPipelineStateNativePtr;
};

class SD3D12GraphicsPipelineState : public IRDIGraphicsPipelineState, public SD3D12PipelineStateBase
{
public:
	SD3D12GraphicsPipelineState(void* _nativePtr) noexcept
		:SD3D12PipelineStateBase(_nativePtr)
	{
	}

	void* GetNativePtr() noexcept { return mPipelineStateNativePtr; }

public:
	SBufferView GetCachedBlob() noexcept override { return SBufferView(mCachedBlob); }
};


class SD3D12ComputePipelineState : public IRDIComputePipelineState, public SD3D12PipelineStateBase
{
public:
	SD3D12ComputePipelineState(void* _nativePtr) noexcept
		:SD3D12PipelineStateBase(_nativePtr)
	{
	}

	void* GetNativePtr() noexcept { return mPipelineStateNativePtr; }

public:
	SBufferView GetCachedBlob() noexcept override { return SBufferView(mCachedBlob); }
};

#pragma once

#include "D3D/D3DUtil.h"
#include "Core/Container/Blob.h"
#include "RDI/Interface/RDIPipelineState.h"

#include "Core/Misc/PreWindowsApi.h"
#include <d3d12.h>
#include "Core/Misc/PostWindowsApi.h"

class SD3D12Device;

class SD3D12GraphicsPipelineState : public IRDIGraphicsPipelineState
{
public:
	void Init(ID3D12PipelineState* _nativePtr, SD3D12Device* _device) noexcept;
	ID3D12PipelineState* GetNativePtr() noexcept { return mPipelineStateNativePtr; }

public:
	SConstBufferView GetCachedBlob() noexcept override { return SConstBufferView(mCachedBlob); }
	void Release() noexcept override;

private:
	SD3D12Device* mDevice = nullptr;
	SBlob mCachedBlob;
	ID3D12PipelineState* mPipelineStateNativePtr = nullptr;
};


class SD3D12ComputePipelineState : public IRDIComputePipelineState
{
public:
	void Init(ID3D12PipelineState* _nativePtr, SD3D12Device* _device) noexcept;
	ID3D12PipelineState* GetNativePtr() noexcept { return mPipelineStateNativePtr; }

public:
	SConstBufferView GetCachedBlob() noexcept override { return SConstBufferView(mCachedBlob); }
	void Release() noexcept override;

private:
	SD3D12Device* mDevice = nullptr;
	SBlob mCachedBlob;
	ID3D12PipelineState* mPipelineStateNativePtr = nullptr;
};

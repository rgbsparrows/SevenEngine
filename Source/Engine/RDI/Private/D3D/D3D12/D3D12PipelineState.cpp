#include "D3D12PipelineState.h"
#include "D3D12Device.h"

void SD3D12GraphicsPipelineState::Init(ID3D12PipelineState* _nativePtr, ID3D12RootSignature* _rootSignatureNativePtr, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mPipelineStateNativePtr = _nativePtr;
	mRootSignatureNativePtr = _rootSignatureNativePtr;

	ID3DBlob* cachedBlob = nullptr;
	mPipelineStateNativePtr->GetCachedBlob(&cachedBlob);

	mCachedBlob.ResizeBlob(cachedBlob->GetBufferPointer(), cachedBlob->GetBufferSize());
	if (cachedBlob != nullptr)
		cachedBlob->Release();
}

void SD3D12GraphicsPipelineState::Release() noexcept
{
	mDevice->ReleaseGraphicsPipelineState(this);
}

void SD3D12ComputePipelineState::Init(ID3D12PipelineState* _nativePtr, ID3D12RootSignature* _rootSignatureNativePtr, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mPipelineStateNativePtr = _nativePtr;
	mRootSignatureNativePtr = _rootSignatureNativePtr;

	ID3DBlob* cachedBlob = nullptr;
	mPipelineStateNativePtr->GetCachedBlob(&cachedBlob);

	mCachedBlob.ResizeBlob(cachedBlob->GetBufferPointer(), cachedBlob->GetBufferSize());
	if (cachedBlob != nullptr)
		cachedBlob->Release();
}

void SD3D12ComputePipelineState::Release() noexcept
{
	mDevice->ReleaseComputePipelineState(this);
}

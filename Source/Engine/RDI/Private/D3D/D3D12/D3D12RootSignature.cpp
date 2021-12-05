#include "D3D12RootSignature.h"
#include "D3D12Device.h"

void SD3D12RootSignature::Init(const void* _rootSignatureBuffer, size_t _bufferSize, ID3D12RootSignature* _nativePtr, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mRootSignatureNativePtr = _nativePtr;
	mBlob.ResizeBlob(_rootSignatureBuffer, _bufferSize);
}

void SD3D12RootSignature::Release() noexcept
{
	mDevice->ReleaseRootSignature(this);
}

#pragma once

#include "Core/Container/Blob.h"
#include "RDI/Interface/RDIRootSignature.h"

class SD3D12RootSignature : public IRDIRootSignature
{
public:
	SD3D12RootSignature(const void* _rootSignatureBuffer, size_t _bufferSize, ID3D12RootSignature* _nativePtr) noexcept
	{
		mRootSignatureNativePtr = _nativePtr;
		mBlob.ResizeBlob(_rootSignatureBuffer, _bufferSize);
	}

	ID3D12RootSignature* GetNativePtr() noexcept { return mRootSignatureNativePtr; }

public:
	SBufferView GetSerializedRootSignature() noexcept override { return SBufferView(mBlob); }

private:
	SBlob mBlob;
	ID3D12RootSignature* mRootSignatureNativePtr;
};

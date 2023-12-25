#pragma once

#include "Core/Container/Blob.h"
#include "RDI/Interface/RDIRootSignature.h"

#include "Core/Misc/PreWindowsApi.h"
#include <d3d12.h>
#include "Core/Misc/PostWindowsApi.h"

class SD3D12Device;

class SD3D12RootSignature : public IRDIRootSignature
{
public:
	void Init(const void* _rootSignatureBuffer, size_t _bufferSize, ID3D12RootSignature* _nativePtr, SD3D12Device* _device) noexcept;
	ID3D12RootSignature* GetNativePtr() noexcept { return mRootSignatureNativePtr; }

public:
	SConstBufferView GetSerializedRootSignature() noexcept override { return SConstBufferView(mBlob); }
	void Release() noexcept override;

private:
	SD3D12Device* mDevice = nullptr;
	SBlob mBlob;
	ID3D12RootSignature* mRootSignatureNativePtr = nullptr;
};

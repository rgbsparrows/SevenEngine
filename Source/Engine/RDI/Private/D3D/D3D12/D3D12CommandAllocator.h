#pragma once

#include "RDI/Interface/RDICommandAllocator.h"

#include "Core/Misc/PreWindowsApi.h"
#include <d3d12.h>
#include "Core/Misc/PostWindowsApi.h"

class SD3D12Device;

class SD3D12CommandAllocator : public IRDICommandAllocator
{
public:
	void Init(ID3D12CommandAllocator* _nativePtr, SD3D12Device* _device) noexcept;
	ID3D12CommandAllocator* GetNativePtr() noexcept { return mCommandAllocatorNativePtr; }

public:
	void Reset() noexcept override;
	void Release() noexcept override;

private:
	ID3D12CommandAllocator* mCommandAllocatorNativePtr = nullptr;
	SD3D12Device* mDevice = nullptr;
};

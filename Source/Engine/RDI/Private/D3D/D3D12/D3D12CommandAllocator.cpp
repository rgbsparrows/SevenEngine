#include "D3D12CommandAllocator.h"
#include "D3D12Device.h"

void SD3D12CommandAllocator::Init(ID3D12CommandAllocator* _nativePtr, SD3D12Device* _device) noexcept
{
	mCommandAllocatorNativePtr = _nativePtr;
	mDevice = _device;
}

void SD3D12CommandAllocator::Reset() noexcept
{
	GetNativePtr()->Reset();
}

void SD3D12CommandAllocator::Release() noexcept
{
	mDevice->ReleaseCommandAllocator(this);
}


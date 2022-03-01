#pragma once

#include "RDI/Interface/RDICommandQueue.h"

class SD3D12CommandQueue : public IRDICommandQueue
{
public:
	void Init(ID3D12CommandQueue* _commandQueueNativePtr, ID3D12Fence* _fenceNativePtr) noexcept;
	void Clear() noexcept;

	ID3D12CommandQueue* GetCommandQueueNativePtr() noexcept { return mCommandQueueNativePtr; }
	ID3D12Fence* GetFenceNativePtr() noexcept { return mFenceNativePtr; }

public:
	void ExecuteCommandLists(uint32_t _commandListCount, IRDICommandList* const* _commandLists) noexcept override;

	uint64_t Signal() noexcept;
	uint64_t GetCompletedValue() noexcept;

	void YieldUntilCompletion(uint64_t _fenceValue) noexcept;
	void WaitForCompletion(uint64_t _fenceValue) noexcept;

private:
	ID3D12CommandQueue* mCommandQueueNativePtr = nullptr;
	ID3D12Fence* mFenceNativePtr = nullptr;

	uint64_t mCurrentFenceValue = 0;
};

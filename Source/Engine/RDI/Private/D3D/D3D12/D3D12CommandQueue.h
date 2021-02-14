#pragma once

#include "RDI/Interface/RDICommandQueue.h"

class SD3D12CommandQueue : public IRDICommandQueue
{
public:
	void Init(void* _commandQueueNativePtr, void* _fenceNativePtr) noexcept;
	void Clear() noexcept;

	void* GetCommandQueueNativePtr() noexcept { return mCommandQueueNativePtr; }
	void* GetFenceNativePtr() noexcept { return mFenceNativePtr; }

public:
	void ExecuteCommandLists(uint32_t _commandListCount, IRDICommandList* const* _commandLists) noexcept override;

	uint64_t Signal() noexcept;
	uint64_t GetCompletedValue() noexcept;

	void YieldUntilCompletion(uint64_t _fenceValue) noexcept;
	void WaitForCompletion(uint64_t _fenceValue) noexcept;

private:
	void* mCommandQueueNativePtr = nullptr;
	void* mFenceNativePtr = nullptr;

	uint64_t mCurrentFenceValue = 0;
};

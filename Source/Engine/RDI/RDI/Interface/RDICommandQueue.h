#pragma once

#include <stdint.h>

__interface IRDICommandList;

__interface IRDICommandQueue
{
	void ExecuteCommandLists(uint32_t _commandListCount, IRDICommandList* const* _commandLists) noexcept;

	uint64_t Signal() noexcept;
	uint64_t GetCompletedValue() noexcept;
	
	void YieldUntilCompletion(uint64_t _fenceValue) noexcept;
	void WaitForCompletion(uint64_t _fenceValue) noexcept;
};

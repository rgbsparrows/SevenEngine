#pragma once

#include "RDI/Interface/RDICommandQueue.h"

#include <vector>

__interface IRDIDevice;
__interface IRDICommandQueue;

__interface IRDICommandList;
__interface IRDICommandAllocator;

class SRenderContent
{
public:
	void Init(IRDIDevice* _device, IRDICommandQueue* _commandQueue, size_t _threadCount) noexcept;
	void Clear() noexcept;

	IRDICommandList* AllocateCommandList() noexcept;
	IRDICommandAllocator* GetCommandAllocator(uint32_t _threadIndex = 0) noexcept
	{
		return mCommandAllocatorList[_threadIndex];
	}

	void ExecuateCommandList(uint32_t _commandListCount, IRDICommandList* const* _commandLists) noexcept
	{
		mRDICommandQueue->ExecuteCommandLists(_commandListCount, _commandLists);
	}

private:
	IRDIDevice* mRDIDevice = nullptr;
	IRDICommandQueue* mRDICommandQueue = nullptr;

	std::vector<IRDICommandAllocator*> mCommandAllocatorList;
};

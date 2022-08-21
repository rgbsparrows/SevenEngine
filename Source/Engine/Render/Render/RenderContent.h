#pragma once

#include "Render/RenderModule.h"

#include <vector>

__interface IRDIDevice;
__interface IRDICommandQueue;

__interface IRDICommandList;
__interface IRDICommandAllocator;

class SRenderContent
{
public:
	void Init(IRDIDevice* _device, IRDICommandQueue* _commandQueue) noexcept;
	void Clear() noexcept;

	void BeginFrame() noexcept;
	void EndFrame() noexcept;

	void EnsureThreadCount(uint32_t _threadCount) noexcept;

	IRDIDevice* GetDevice() noexcept;

	IRDICommandList* AllocateCommandList(uint32_t _threadIndex = 0) noexcept;
	void ReleaseCommandList(IRDICommandList* _commandList) noexcept;
	IRDICommandAllocator* GetCommandAllocator(uint32_t _threadIndex = 0) noexcept;

	void ExecuteCommandList(IRDICommandList* _commandList) noexcept;
	void ExecuteCommandLists(uint32_t _commandListCount, IRDICommandList* const* _commandLists) noexcept;

	void SyncToGpuFrameEnd(bool _force = false) noexcept;

private:
	IRDIDevice* mRDIDevice = nullptr;
	IRDICommandQueue* mRDICommandQueue = nullptr;

	bool mIsSyncToGpuFrameEnd = false;

	std::vector<IRDICommandAllocator*> mCommandAllocatorList[GRenderInfoCount];
	std::vector<IRDICommandList*> mCommandListPool;
};

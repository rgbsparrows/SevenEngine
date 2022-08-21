#pragma once
#include "Render/RenderContent.h"

class SSubRenderContent
{
public:
	void Init(SRenderContent* _renderContent, uint32_t _threadCount) noexcept;
	void Clear() noexcept;

	IRDIDevice* GetDevice() noexcept
	{
		return mRenderContent->GetDevice();
	}

	IRDICommandList* AllocateCommandList(uint32_t _threadIndex = 0) noexcept
	{
		return mRenderContent->AllocateCommandList(_threadIndex);
	}

	IRDICommandAllocator* GetCommandAllocator(uint32_t _threadIndex = 0) noexcept
	{
		return mRenderContent->GetCommandAllocator(_threadIndex);
	}

	void ExecuteCommandLists(uint32_t _commandListCount, IRDICommandList* const* _commandLists) noexcept
	{
		mRenderContent->ExecuteCommandLists(_commandListCount, _commandLists);
	}

	void SyncToGpuFrameEnd(bool _force = false) noexcept
	{
		mRenderContent->SyncToGpuFrameEnd(_force);
	}

private:
	SRenderContent* mRenderContent;
};

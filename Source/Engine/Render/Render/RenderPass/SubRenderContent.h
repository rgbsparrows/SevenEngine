#pragma once
#include "Render/RenderContent.h"

class SSubRenderContent
{
public:
	void Init(SRenderContent* _renderContent, uint32_t _threadCount) noexcept;
	void Clear() noexcept;

	IRDICommandList* AllocateCommandList() noexcept
	{
		return mRenderContent->AllocateCommandList();
	}

	IRDICommandAllocator* GetCommandAllocator(uint32_t _threadIndex = 0) noexcept
	{
		return mRenderContent->GetCommandAllocator(_threadIndex);
	}

	void ExecuteCommandLists(uint32_t _commandListCount, IRDICommandList* const* _commandLists) noexcept
	{
		mRenderContent->ExecuteCommandLists(_commandListCount, _commandLists);
	}

private:
	SRenderContent* mRenderContent;
};

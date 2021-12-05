#pragma once

#include "Core/Container/Blob.h"

__interface IRDIGraphicsPipelineState
{
	SBufferView GetCachedBlob() noexcept;
	void Release() noexcept;
};

__interface IRDIComputePipelineState
{
	SBufferView GetCachedBlob() noexcept;
	void Release() noexcept;
};

#pragma once

#include "Core/Container/Blob.h"

__interface IRDIGraphicsPipelineState
{
	SConstBufferView GetCachedBlob() noexcept;
	void Release() noexcept;
};

__interface IRDIComputePipelineState
{
	SConstBufferView GetCachedBlob() noexcept;
	void Release() noexcept;
};

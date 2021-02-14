#pragma once

#include "Core/Container/Blob.h"

__interface IRDIPipelineState
{
	SBufferView GetCachedBlob() noexcept;
};

__interface IRDIGraphicsPipelineState : IRDIPipelineState
{
};

__interface IRDIComputePipelineState : IRDIPipelineState
{
};

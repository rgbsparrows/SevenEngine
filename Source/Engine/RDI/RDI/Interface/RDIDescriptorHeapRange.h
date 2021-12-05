#pragma once

#include <stdint.h>

__interface IRDIShaderResourceView;
__interface IRDIUnorderedAccessView;
__interface IRDISamplerView;

__interface IRDIDescriptorHeapRange
{
	void SetSRV(uint64_t _offset, IRDIShaderResourceView* _srv) noexcept;
	void SetUAV(uint64_t _offset, IRDIUnorderedAccessView* _uav) noexcept;
	void Release() noexcept;
};

__interface IRDISamplerHeapRange
{
	void SetSampler(uint64_t _offset, IRDISamplerView* _sampler) noexcept;
	void Release() noexcept;
};

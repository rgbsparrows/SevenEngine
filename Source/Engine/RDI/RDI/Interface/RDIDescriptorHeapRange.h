#pragma once

#include <stdint.h>

__interface IRDIShaderResourceView;
__interface IRDIUnorderedAccessView;
__interface IRDISamplerView;

__interface IRDIDescriptorHeapRange
{
	void SetSRV(IRDIShaderResourceView* _srv, uint64_t _offset) noexcept;
	void SetUAV(IRDIUnorderedAccessView* _uav, uint64_t _offset) noexcept;
	void Release() noexcept;
};

__interface IRDISamplerHeapRange
{
	void SetSampler(IRDISamplerView* _sampler, uint64_t _offset) noexcept;
	void Release() noexcept;
};

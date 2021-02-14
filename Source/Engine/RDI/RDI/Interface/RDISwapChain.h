#pragma once

#include <stdint.h>

struct SRDISwapChainDesc;

__interface IRDIOutput;
__interface IRDITexture2D;

__interface IRDISwapChain
{
	void GetDesc(SRDISwapChainDesc* _desc)const noexcept;

	IRDITexture2D* GetRenderTarget(uint32_t _index) noexcept;
	IRDIOutput* GetOutput() noexcept;
	void Present() noexcept;
};

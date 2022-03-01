#pragma once

#include <stdint.h>

struct SRDISwapChainDesc;

__interface IRDIOutput;
__interface IRDITexture2D;

__interface IRDISwapChain
{
	void GetDesc(SRDISwapChainDesc* _desc)const noexcept;
	void Release() noexcept;

	IRDITexture2D* GetRenderTarget() noexcept;
	IRDIOutput* GetOutput() noexcept;
	void Present(bool _sync) noexcept;
};

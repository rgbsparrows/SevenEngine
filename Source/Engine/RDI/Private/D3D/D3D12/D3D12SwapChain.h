#pragma once

#include "RDI/RDIFunctionHelper.h"
#include "D3D/D3D12/D3D12Output.h"
#include "RDI/Interface/RDISwapChain.h"
#include "D3D/D3D12/Helper/D3D12Helper.h"

class SD3D12Device;
class SD3D12Output;
class SD3D12Adapter;

class SD3D12SwapChain : public IRDISwapChain
{
public:
	void Init(IDXGISwapChain* _nativePtr, const SRDISwapChainDesc* _desc, SD3D12Device* _device, SD3D12Adapter* _adapter) noexcept;
	IDXGISwapChain* GetNativePtr() noexcept { return mSwapChainNativePtr; }

public:
	void GetDesc(SRDISwapChainDesc* _desc)const noexcept override { *_desc = mDesc; }
	void Release() noexcept override;

	IRDITexture2D* GetRenderTarget() noexcept override { return mRenderTargets[mCurrentBackBufferIndex]; }
	IRDIOutput* GetOutput() noexcept override { return mOutput; }
	void Present(bool _sync) noexcept override;

private:
	IDXGISwapChain* mSwapChainNativePtr = nullptr;
	SRDISwapChainDesc mDesc;

	SD3D12Device* mDevice = nullptr;
	SD3D12Adapter* mAdapter = nullptr;

	IRDITexture2D* mRenderTargets[D3D12_BACKBUFFER_COUNT] = {};
	SD3D12Output* mOutput = nullptr;

	uint32_t mCurrentBackBufferIndex = 0;
};

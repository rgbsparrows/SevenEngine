#pragma once

#include "RDI/RDIFunctionHelper.h"
#include "RDI/Interface/RDISwapChain.h"
#include "D3D/D3D12/Warper/D3D12ImplWarperHelper.h"
#include "D3D/D3D12/D3D12Output.h"

class SD3D12Device;
class SD3D12Output;
class SD3D12Adapter;

class SD3D12SwapChain : public IRDISwapChain
{
public:
	void Init(void* _nativePtr, const SRDISwapChainDesc* _desc, SD3D12Device* _device, SD3D12Adapter* _adapter) noexcept;
	void* GetNativePtr() noexcept { return mSwapChainNativePtr; }

public:
	void GetDesc(SRDISwapChainDesc* _desc)const noexcept override { *_desc = mDesc; }

	IRDITexture2D* GetRenderTarget(uint32_t _index) noexcept override { return mRenderTargets[_index]; }
	IRDIOutput* GetOutput() noexcept override { return mOutput; }
	void Present() noexcept override;

private:
	void* mSwapChainNativePtr = nullptr;
	SRDISwapChainDesc mDesc;

	SD3D12Device* mDevice = nullptr;
	SD3D12Adapter* mAdapter = nullptr;

	IRDITexture2D* mRenderTargets[D3D12_BACKBUFFER_COUNT] = {};
	SD3D12Output* mOutput = nullptr;
};

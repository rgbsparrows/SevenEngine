#pragma once

#include "RDI/Interface/RDIFactory.h"

#include "D3D/D3D12/D3D12Device.h"
#include "D3D/D3D12/D3D12Adapter.h"


class SD3D12Factory : public IRDIFactory
{
public:
	bool Init() noexcept;
	void Clear() noexcept;

	IDXGIFactory4* GetNativePtr() noexcept { return mDXGIFactoryNativePtr; }
	ID3D12Debug* GetDebugNativePtr() noexcept { return mD3D12DebugNativePtr; }

public:
	const SRDIFutureSupport& GetFutureSupport() noexcept { return mFutureSupport; }
	std::wstring_view GetFactoryDesc() noexcept { return L"D3D12 RDI"; }

	bool Create() noexcept override { return Init(); }
	void Release() noexcept override { Release(); }

	IRDIDevice* GetDevice() noexcept override { return &mD3D12Device; }
	const std::vector<IRDIAdapter*>& GetAdapters() noexcept override { return mCachedAdapters; }

private:
	std::vector<IRDIAdapter*> mCachedAdapters;

private:
	IDXGIFactory4* mDXGIFactoryNativePtr = nullptr;
	ID3D12Debug* mD3D12DebugNativePtr = nullptr;

	SD3D12Device mD3D12Device;
	std::vector<SD3D12Adapter> mAdpaters;

	SRDIFutureSupport mFutureSupport = {};
};

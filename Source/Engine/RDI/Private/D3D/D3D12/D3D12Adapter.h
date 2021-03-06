#pragma once

#include "RDI/Interface/RDIAdapter.h"
#include "RDI/RDIFunctionHelper.h"

#include "D3D/D3D12/D3D12Output.h"

struct SD3D12AdapterDesc
{
	std::wstring mDescription;
	uint64_t mDedicatedVideoMemory = 0;
	uint64_t mDedicatedSystemMemory = 0;
	uint64_t mSharedSystemMemory = 0;
};

class SD3D12Adapter : public IRDIAdapter
{
public:
	void Init(void* _nativePtr) noexcept;
	void Clear() noexcept;

	void* GetNativePtr() noexcept { return mAdapterNativePtr; }

public:
	void GetDesc(SRDIAdapterDesc* _desc) const noexcept override { *_desc = mCachedDesc; }
	const std::vector<IRDIOutput*>& GetOutputs() const noexcept override { return mCachedOutputs; }

private:
	std::vector<IRDIOutput*> mCachedOutputs;
	SRDIAdapterDesc mCachedDesc;

private:
	void* mAdapterNativePtr = nullptr;
	SD3D12AdapterDesc mDesc;
	std::vector<SD3D12Output> mOutputs;
};

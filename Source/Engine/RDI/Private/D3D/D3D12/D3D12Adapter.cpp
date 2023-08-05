#include "D3D/D3DUtil.h"
#include "Core/Misc/Localize.h"
#include "D3D/D3D12/D3D12Adapter.h"

void SD3D12Adapter::Init(IDXGIAdapter* _nativePtr) noexcept
{
	{
		mAdapterNativePtr = _nativePtr;
	}

	//Desc
	{
		DXGI_ADAPTER_DESC desc;
		GetNativePtr()->GetDesc(&desc);

		mDesc.mDescription = Locale::ConvertWstringToString(desc.Description);
		mDesc.mDedicatedVideoMemory = desc.DedicatedVideoMemory;
		mDesc.mDedicatedSystemMemory = desc.DedicatedSystemMemory;
		mDesc.mSharedSystemMemory = desc.SharedSystemMemory;

		mCachedDesc.mDescription = mDesc.mDescription;
		mCachedDesc.mDedicatedVideoMemory = mDesc.mDedicatedVideoMemory;
		mCachedDesc.mDedicatedSystemMemory = mDesc.mDedicatedSystemMemory;
		mCachedDesc.mSharedSystemMemory = mDesc.mSharedSystemMemory;
	}

	//Outputs
	{
		uint32_t res = 0;
		uint32_t index = 0;
		IDXGIOutput* outputNativePtr = nullptr;

		for (void(); GetNativePtr()->EnumOutputs(index, &outputNativePtr) == S_OK; ++index)
		{
			mOutputs.push_back(SD3D12Output());

			mOutputs.back().Init(outputNativePtr);
		}

		mCachedOutputs.resize(mOutputs.size());
		for (size_t i = 0; i != mOutputs.size(); ++i)
			mCachedOutputs[i] = &mOutputs[i];
	}
}

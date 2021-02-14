#include "D3D/D3DUtil.h"
#include "D3D/D3D12/D3D12Adapter.h"
#include "D3D/Warper/D3DImplWarper.h"
#include "D3D/D3D12/Warper/D3D12ImplWarper.h"

void SD3D12Adapter::Init(void* _nativePtr) noexcept
{
	{
		mAdapterNativePtr = _nativePtr;
	}

	//Desc
	{
		VERIFY_D3D_RETURN(D3DAPIWarp_Impl::GetDXGIAdapterDesc_D3DImpl(GetNativePtr(), &mDesc.mDescription, &mDesc.mDedicatedVideoMemory, &mDesc.mDedicatedSystemMemory, &mDesc.mSharedSystemMemory));
		mCachedDesc.mDescription = mDesc.mDescription;
		mCachedDesc.mDedicatedVideoMemory = mDesc.mDedicatedVideoMemory;
		mCachedDesc.mDedicatedSystemMemory = mDesc.mDedicatedSystemMemory;
		mCachedDesc.mSharedSystemMemory = mDesc.mSharedSystemMemory;
	}

	//Outputs
	{
		uint32_t res = 0;
		uint32_t index = 0;
		void* outputNativePtr = nullptr;

		for (void(); D3DAPIWarp_Impl::EnumDXGIOutput_D3DImpl(GetNativePtr(), index, &outputNativePtr) == 0; ++index)
		{
			mOutputs.push_back(SD3D12Output());

			mOutputs.back().Init(outputNativePtr);
		}

		mCachedOutputs.resize(mOutputs.size());
		for (size_t i = 0; i != mOutputs.size(); ++i)
			mCachedOutputs[i] = &mOutputs[i];
	}
}

#include "D3D/D3DUtil.h"
#include "D3D/D3D12/D3D12Factory.h"
#include "D3D/Warper/D3DImplWarper.h"
#include "D3D/D3D12/Warper/D3D12ImplWarper.h"
#include "Core/BuildConfiguation/BuildConfiguation.h"

bool SD3D12Factory::Init() noexcept
{
	//DebugLayer
	{
#if WITH_DEBUG_CODE
		VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12GetDebugInterface_D3D12Impl(&mD3D12DebugNativePtr));
		D3D12APIWarp_Impl::D3D12EnableDebugLayer_D3D12Impl(GetDebugNativePtr());
		D3D12APIWarp_Impl::D3D12SetEnableGPUBasedValidation_D3D12Impl(GetDebugNativePtr());
#endif
	}

	//DXGIFactory
	{
		VERIFY_D3D_RETURN(D3DAPIWarp_Impl::CreateDXGIFactory_D3DImpl(SBuildConfiguation::GIsDebugMode, &mDXGIFactoryNativePtr));
	}

	//Adapter
	{
		//Hardware Adapter
		{
			uint32_t res = 0;
			uint32_t index = 0;
			void* adapterNativePtr = nullptr;

			for (void(); D3DAPIWarp_Impl::EnumDXGIAdapter_D3DImpl(GetNativePtr(), index, &adapterNativePtr) == 0; ++index)
			{
				mAdpaters.push_back(SD3D12Adapter());

				mAdpaters.back().Init(adapterNativePtr);
			}
		}

		//Warp Adapter
		if (mAdpaters.empty())
		{
			void* warpAdapterNativePtr = nullptr;
			VERIFY_D3D_RETURN(D3DAPIWarp_Impl::EnumDXGIWarpAdapter_D3DImpl(GetNativePtr(), &warpAdapterNativePtr));

			mAdpaters.push_back(SD3D12Adapter());

			mAdpaters.back().Init(warpAdapterNativePtr);
		}

		mCachedAdapters.resize(mAdpaters.size());
		for (size_t i = 0; i != mAdpaters.size(); ++i)
			mCachedAdapters[i] = &mAdpaters[i];
	}

	//Device
	{
		void* d3d12DeviceNativePtr = nullptr;
		VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12CreateDevice_D3D12Impl(mAdpaters[0].GetNativePtr(), &d3d12DeviceNativePtr));
		mD3D12Device.Init(d3d12DeviceNativePtr, &mAdpaters[0], this);
	}

	return true;
}

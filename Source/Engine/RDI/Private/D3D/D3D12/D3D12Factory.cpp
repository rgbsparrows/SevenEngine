#include "D3D/D3DUtil.h"
#include "D3D/D3D12/D3D12Factory.h"
#include "Core/ProgramConfiguation/BuildConfiguation.h"

#pragma comment(lib, "dxgi.lib")

bool SD3D12Factory::Init() noexcept
{
	//DebugLayer
	{
#if WITH_DEBUG_CODE
		VERIFY_D3D_RETURN(D3D12GetDebugInterface(IID_PPV_ARGS(&mD3D12DebugNativePtr)));
		GetDebugNativePtr()->EnableDebugLayer();
#endif
	}

	//DXGIFactory
	{
		VERIFY_D3D_RETURN(CreateDXGIFactory(IID_PPV_ARGS(&mDXGIFactoryNativePtr)));
	}

	//Adapter
	{
		//Hardware Adapter
		{
			uint32_t res = 0;
			uint32_t index = 0;
			IDXGIAdapter* adapterNativePtr = nullptr;

			for(void(); mDXGIFactoryNativePtr->EnumAdapters(index, &adapterNativePtr) == S_OK; ++index)
			{
				mAdpaters.push_back(SD3D12Adapter());
				mAdpaters.back().Init(adapterNativePtr);
			}
		}

		//Warp Adapter
		if (mAdpaters.empty())
		{
			IDXGIAdapter* warpAdapterNativePtr = nullptr;
			VERIFY_D3D_RETURN(mDXGIFactoryNativePtr->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapterNativePtr)));

			mAdpaters.push_back(SD3D12Adapter());
			mAdpaters.back().Init(warpAdapterNativePtr);
		}

		mCachedAdapters.resize(mAdpaters.size());
		for (size_t i = 0; i != mAdpaters.size(); ++i)
			mCachedAdapters[i] = &mAdpaters[i];
	}

	//Device
	{
		ID3D12Device* d3d12DeviceNativePtr = nullptr;
		D3D12CreateDevice(mAdpaters[0].GetNativePtr(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&d3d12DeviceNativePtr));

		mD3D12Device.Init(d3d12DeviceNativePtr, &mAdpaters[0], this);
	}

	return true;
}

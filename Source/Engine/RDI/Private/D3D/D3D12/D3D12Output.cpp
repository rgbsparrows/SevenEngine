#include "D3D/D3DUtil.h"
#include "D3D/D3D12/D3D12Output.h"
#include "D3D/Warper/D3DImplWarper.h"
#include "D3D/D3D12/Warper/D3D12ImplWarper.h"

void SD3D12Output::Init(void* _nativePtr) noexcept
{
	{
		mOutputNativePtr = _nativePtr;
	}

	//Desc
	{
		VERIFY_D3D_RETURN(D3DAPIWarp_Impl::GetDXGIOutputDesc_D3DImpl(GetNativePtr(), &mDesc.mDeviceName, &mDesc.mDesktopCoordinates.mLeftOn[0], &mDesc.mDesktopCoordinates.mLeftOn[1], &mDesc.mDesktopCoordinates.mRightDwon[0], &mDesc.mDesktopCoordinates.mRightDwon[1], &mDesc.mAttachedToDesktop));
		mCachedDesc.mDeviceName = mDesc.mDeviceName;
		mCachedDesc.mDesktopCoordinates = mDesc.mDesktopCoordinates;
		mCachedDesc.mAttachedToDesktop = mDesc.mAttachedToDesktop;
	}
}

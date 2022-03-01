#include "D3D/D3DUtil.h"
#include "D3D/D3D12/D3D12Output.h"

void SD3D12Output::Init(IDXGIOutput* _nativePtr) noexcept
{
	{
		mOutputNativePtr = _nativePtr;
	}

	//Desc
	{
		DXGI_OUTPUT_DESC desc;
		GetNativePtr()->GetDesc(&desc);

		mDesc.mDeviceName = desc.DeviceName;
		mDesc.mDesktopCoordinates.mLeftOn[0] = desc.DesktopCoordinates.left;
		mDesc.mDesktopCoordinates.mLeftOn[1] = desc.DesktopCoordinates.top;
		mDesc.mDesktopCoordinates.mRightDwon[0] = desc.DesktopCoordinates.right;
		mDesc.mDesktopCoordinates.mRightDwon[1] = desc.DesktopCoordinates.bottom;

		mCachedDesc.mDeviceName = mDesc.mDeviceName;
		mCachedDesc.mDesktopCoordinates = mDesc.mDesktopCoordinates;
		mCachedDesc.mAttachedToDesktop = mDesc.mAttachedToDesktop;
	}
}

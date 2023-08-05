#pragma once

#include "RDI/Interface/RDIOutput.h"
#include "RDI/RDIFunctionHelper.h"

#include "Core/Misc/PreWindowsApi.h"
#include <dxgi1_6.h>
#include "Core/Misc/PostWindowsApi.h"

struct SD3D12OutputDesc
{
	std::string mDeviceName;
	Math::SIntRect mDesktopCoordinates;
	bool mAttachedToDesktop = false;
};

class SD3D12Output : public IRDIOutput
{
public:
	void Init(IDXGIOutput* _nativePtr) noexcept;
	void Clear() noexcept;

	IDXGIOutput* GetNativePtr() noexcept { return mOutputNativePtr; }

public:

	void GetDesc(SRDIOutputDesc* _desc) noexcept { *_desc = mCachedDesc; }

private:
	SRDIOutputDesc mCachedDesc;

private:
	IDXGIOutput* mOutputNativePtr = nullptr;
	SD3D12OutputDesc mDesc;
};

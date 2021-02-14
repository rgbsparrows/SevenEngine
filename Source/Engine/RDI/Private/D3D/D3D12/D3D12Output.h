#pragma once

#include "RDI/Interface/RDIOutput.h"

#include "RDI/RDIFunctionHelper.h"

struct SD3D12OutputDesc
{
	std::wstring mDeviceName;
	Math::SIntRect mDesktopCoordinates;
	bool mAttachedToDesktop = false;
};

class SD3D12Output : public IRDIOutput
{
public:
	void Init(void* _nativePtr) noexcept;
	void Clear() noexcept;

	void* GetNativePtr() noexcept { return mOutputNativePtr; }

public:

	void GetDesc(SRDIOutputDesc* _desc) noexcept { *_desc = mCachedDesc; }

private:
	SRDIOutputDesc mCachedDesc;


private:
	void* mOutputNativePtr = nullptr;
	SD3D12OutputDesc mDesc;
};

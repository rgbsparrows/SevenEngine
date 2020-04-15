#include "ErrorCode.h"
#include "WindowsPlatformApiImplWarper.h"

std::wstring SErrorCode::GetDesc() const noexcept
{
	return WindowsPlatformApiWarp_Impl::FormatMessage_WindowsImpl(static_cast<uint32_t>(mErrorCode));
}

#include "Core/Localize.h"
#include "WindowsPlatform/WindowsPlatformApi.h"

namespace Locale
{
	std::wstring ConvertStringToWstring(uint32_t _codePage, std::string_view _srcString) noexcept
	{
		return SWindowsPlatformApi::ConvertStringToWstring(_codePage, _srcString);
	}

	std::string ConvertWstringToString(uint32_t _codePage, std::wstring_view _srcString) noexcept
	{
		return SWindowsPlatformApi::ConvertWstringToString(_codePage, _srcString);
	}
}

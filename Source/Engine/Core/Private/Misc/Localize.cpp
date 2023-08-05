#include "Core/Misc/Localize.h"
#include "Core/Misc/windowsEx.h"

namespace Locale
{
	std::wstring ConvertStringToWstring(std::string_view _srcString, uint32_t _codePage) noexcept
	{
		std::wstring result;

		size_t wideCharCount = ::MultiByteToWideChar(_codePage, 0, _srcString.data(), static_cast<int>(_srcString.size()), nullptr, 0);

		result.resize(wideCharCount);
		::MultiByteToWideChar(_codePage, 0, _srcString.data(), static_cast<int>(_srcString.size()), &result[0], static_cast<int>(result.size()));

		return result;
	}

	std::string ConvertWstringToString(std::wstring_view _srcString, uint32_t _codePage) noexcept
	{
		std::string result;

		int32_t useDefaultChar;
		size_t mutiCharCount = ::WideCharToMultiByte(_codePage, 0, _srcString.data(), static_cast<int>(_srcString.size()), nullptr, 0, "\0", &useDefaultChar);

		result.resize(mutiCharCount);
		::WideCharToMultiByte(_codePage, 0, _srcString.data(), static_cast<int>(_srcString.size()), &result[0], static_cast<int>(result.size()), "\0", &useDefaultChar);

		return result;
	}
}

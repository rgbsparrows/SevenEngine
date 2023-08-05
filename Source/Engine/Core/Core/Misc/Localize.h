#pragma once

#include <string>

namespace Locale
{
	namespace ECodePage
	{
		static constexpr uint32_t ACP = 0;
		static constexpr uint32_t OEMCP = 1;
		static constexpr uint32_t MACCP = 2;
		static constexpr uint32_t THREAD_ACP = 3;
		static constexpr uint32_t SYMBOL = 42;
		static constexpr uint32_t UTF7 = 65000;
		static constexpr uint32_t UTF8 = 65001;
		static constexpr uint32_t GBK = 936;
	}

	std::wstring ConvertStringToWstring(std::string_view _srcString, uint32_t _codePage = ECodePage::UTF8) noexcept;
	std::string ConvertWstringToString(std::wstring_view _srcString, uint32_t _codePage = ECodePage::UTF8) noexcept;
}

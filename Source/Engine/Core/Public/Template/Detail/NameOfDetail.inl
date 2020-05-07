#pragma once

#include "Macros/UtilMacros.h"

#include <string_view>
#include <stdint.h>

namespace NameOfDetail
{
	template<typename _type> 
	NO_DISCARD_RETURN constexpr std::wstring_view GetFullTypeName() noexcept
	{
		std::wstring_view funcsig = MAKE_WIDE(__FUNCSIG__);
		funcsig.remove_prefix(86);
		funcsig.remove_suffix(16);
		return funcsig;
	}

	NO_DISCARD_RETURN constexpr uint64_t CalcStrHash(std::wstring_view _str) noexcept
	{
		constexpr uint64_t FnvPrime = 1099511628211ULL;
		constexpr uint64_t FnvOffsetBasis = 14695981039346656037ULL;

		uint64_t val = FnvOffsetBasis;
		for (size_t i = 0; i != _str.size(); ++i)
		{
			val ^= static_cast<uint64_t>(_str[i]);
			val *= FnvPrime;
		}
		return val;
	}
}

#pragma once

#include "Core/Macros/UtilMacros.h"
#include "Core/Macros/ConditionDeprecated.h"

#include <stdint.h>
#include <string_view>
#include <type_traits>

namespace NameOfDetail
{
	template<typename _type>
	NO_DISCARD_RETURN constexpr std::wstring_view GetFullTypeName() noexcept
	{
		std::wstring_view funcsig = MAKE_WIDE(__FUNCSIG__);
		funcsig.remove_prefix(110);
		funcsig.remove_suffix(16);
		size_t lastPos = funcsig.find_last_of(L' ');
		if (lastPos != std::wstring_view::npos)
			funcsig.remove_prefix(lastPos + 1);
		return funcsig;
	}

	REWRITE_WHEN_CONCEPT_AVAILABLE(
		"验证_type是否是class，struct，union，enum，enum class，或是基本类型\n"
	)
	template<typename _type>
	NO_DISCARD_RETURN constexpr std::wstring_view GetShortTypeName() noexcept
	{
		std::wstring_view typeName = GetFullTypeName<_type>();
		if constexpr (!std::is_integral_v<_type> && !std::is_floating_point_v<_type>)
		{
			size_t lastPos = typeName.find_last_of(L':');
			if (lastPos != std::wstring_view::npos)
				typeName.remove_prefix(lastPos + 1);
		}
		return typeName;
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

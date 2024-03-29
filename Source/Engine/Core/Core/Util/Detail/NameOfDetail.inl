#pragma once

#include "Core/Util/UtilMacros.h"
#include "Core/ProgramConfiguation/ConditionDeprecated.h"

#include <stdint.h>
#include <string_view>
#include <type_traits>

namespace NameOfDetail
{
	template<typename _type>
	NO_DISCARD_RETURN constexpr std::string_view GetFullTypeName() noexcept
	{
		std::string_view funcsig = __FUNCSIG__;
		funcsig.remove_prefix(110);
		funcsig.remove_suffix(16);
		size_t lastPos = funcsig.find_last_of(' ');
		if (lastPos != std::string_view::npos && !funcsig.contains('<'))
			funcsig.remove_prefix(lastPos + 1);
		return funcsig;
	}

	template<typename _type>
	NO_DISCARD_RETURN constexpr std::string_view GetShortTypeName() noexcept
	{
		std::string_view typeName = GetFullTypeName<_type>();
		if (!std::is_integral_v<_type> && !std::is_floating_point_v<_type> && !typeName.contains('<'))
		{
			size_t lastPos = typeName.find_last_of(':');
			if (lastPos != std::string_view::npos)
				typeName.remove_prefix(lastPos + 1);
		}
		return typeName;
	}

	NO_DISCARD_RETURN constexpr uint64_t CalcStrHash(std::string_view _str) noexcept
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

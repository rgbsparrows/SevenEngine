#pragma once

#include "Core/Util/UtilMacros.h"
#include "Core/Util/StringParser.h"
#include "Core/Util/TemplateUtil.h"

struct SCommandLine
{
	static const std::wstring& GetCommandLine() noexcept;

	static bool HasArgument(std::wstring_view _key) noexcept
	{
		return GetRawArgumentValue(_key).first;
	}

	template<typename _type> NO_DISCARD_RETURN static auto GetArgumentValue(std::wstring_view _key, const _type& _defaultValue = _type()) noexcept
	{
		using Type = _type;
		auto rawValue = GetRawArgumentValue(_key);

		std::pair<bool, Type> res;
		if (rawValue.first) res.first = TStringParser<Type>()(rawValue.second, res.second);
		else res.first = false;

		return res.first ? res : std::make_pair(false, _defaultValue);
	}

	NO_DISCARD_RETURN static std::pair<bool, std::wstring_view> GetRawArgumentValue(std::wstring_view _key) noexcept;
};

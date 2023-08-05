#pragma once

#include "Core/Misc/Localize.h"
#include "Detail/StringParserDetail.inl"

#include <string>
#include <filesystem>
#include <string_view>

template<typename _type>
struct TStringParser
{
	using Type = _type;
	bool operator()(std::string_view _str, Type& _value) noexcept = delete;
};

template<>
struct TStringParser<std::string>
{
	bool operator()(std::string_view _str, std::string_view& _value) noexcept
	{
		_value = _str;
		return true;
	}
};

template<>
struct TStringParser<std::wstring>
{
	bool operator()(std::string_view _str, std::wstring& _value) noexcept
	{
		_value = Locale::ConvertStringToWstring(_str);
		return true;
	}
};

template<>
struct TStringParser<wchar_t>
{
	bool operator()(std::string_view _str, wchar_t& _value) noexcept
	{
		if (_str.size() == 1)
		{
			_value = _str[0];
			return true;
		}
		return false;
	}
};

template<>
struct TStringParser<bool>
{
	bool operator()(std::string_view _str, bool& _value) noexcept
	{
		if (_str == "true") _value = true;
		else if (_str == "false") _value = false;
		
		return false;
	}
};

template<>
struct TStringParser<std::filesystem::path>
{
	bool operator()(std::string_view _str, std::filesystem::path& _value) noexcept
	{
		_value = Locale::ConvertStringToWstring(_str);
		return true;
	}
};

template<> struct TStringParser<int8_t  > : StringParserDetail::TIntegralStringParserHelper<int8_t  > {};
template<> struct TStringParser<int16_t > : StringParserDetail::TIntegralStringParserHelper<int16_t > {};
template<> struct TStringParser<int32_t > : StringParserDetail::TIntegralStringParserHelper<int32_t > {};
template<> struct TStringParser<int64_t > : StringParserDetail::TIntegralStringParserHelper<int64_t > {};

template<> struct TStringParser<uint8_t > : StringParserDetail::TIntegralStringParserHelper<uint8_t > {};
template<> struct TStringParser<uint16_t> : StringParserDetail::TIntegralStringParserHelper<uint16_t> {};
template<> struct TStringParser<uint32_t> : StringParserDetail::TIntegralStringParserHelper<uint32_t> {};
template<> struct TStringParser<uint64_t> : StringParserDetail::TIntegralStringParserHelper<uint64_t> {};

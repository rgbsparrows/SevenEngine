#pragma once

#include "Detail/StringParserDetail.inl"

#include <string>
#include <filesystem>
#include <string_view>

template<typename _type>
struct TStringParser
{
	using Type = _type;
	bool operator()(std::wstring_view _str, Type& _value) noexcept = delete;
};

template<>
struct TStringParser<std::wstring>
{
	bool operator()(std::wstring_view _str, std::wstring_view& _value) noexcept
	{
		_value = _str;
		return true;
	}
};

template<>
struct TStringParser<wchar_t>
{
	bool operator()(std::wstring_view _str, wchar_t& _value) noexcept
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
	bool operator()(std::wstring_view _str, bool& _value) noexcept
	{
		if (_str == L"true") _value = true;
		else if (_str == L"false") _value = false;
		else return false;
		return true;
	}
};

template<>
struct TStringParser<std::filesystem::path>
{
	bool operator()(std::wstring_view _str, std::filesystem::path& _value) noexcept
	{
		_value = _str;
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

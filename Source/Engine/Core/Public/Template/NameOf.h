#pragma once

#include "Detail/NameOfDetail.inl"

NO_DISCARD_RETURN constexpr uint64_t CalcStrHash(std::wstring_view _str)
{
	return NameOfDetail::CalcStrHash(_str);
}

template<typename _type>
constexpr std::wstring_view TFullTypeName = NameOfDetail::GetFullTypeName<_type>();

template<typename _type>
constexpr std::wstring_view TShortTypeName = NameOfDetail::GetShortTypeName<_type>();

template<typename _type>
constexpr uint64_t TTypeHash = CalcStrHash(TFullTypeName<_type>);

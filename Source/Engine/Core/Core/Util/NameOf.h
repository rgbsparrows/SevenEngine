﻿#pragma once

#include "Detail/NameOfDetail.inl"

NO_DISCARD_RETURN constexpr uint64_t CalcStrHash(std::string_view _str) noexcept
{
	return NameOfDetail::CalcStrHash(_str);
}

template<typename _type>
constexpr std::string_view TFullTypeName = NameOfDetail::GetFullTypeName<_type>();

template<typename _type>
constexpr std::string_view TShortTypeName = NameOfDetail::GetShortTypeName<_type>();

template<typename _type>
constexpr uint64_t TTypeHash = CalcStrHash(TFullTypeName<_type>);

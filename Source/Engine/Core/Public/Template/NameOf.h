#pragma once

#include "Detail/NameOfDetail.inl"

template<typename _type>
constexpr std::wstring_view TFullTypeName = NameOfDetail::GetFullTypeName<_type>();

template<typename _type>
constexpr std::wstring_view TShortTypeName = NameOfDetail::GetShortTypeName<_type>();

template<typename _type>
constexpr uint64_t TTypeHash = NameOfDetail::CalcStrHash(TFullTypeName<_type>);

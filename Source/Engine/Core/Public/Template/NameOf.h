#pragma once

#include "Detail/NameOfDetail.inl"

template<typename _type>
constexpr std::wstring_view TFullTypeName = NameOfDetail::GetFullTypeName<_type>();

TODO("没写")
template<typename _type>
constexpr std::wstring_view TShortTypeName = NameOfDetail::GetFullTypeName<_type>();

template<typename _type>
constexpr uint64_t TTypeHash = NameOfDetail::CalcStrHash(TFullTypeName<_type>);

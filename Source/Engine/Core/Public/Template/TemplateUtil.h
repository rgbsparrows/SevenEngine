#pragma once

#include "Detail/TemplateUtilDetail.inl"

#include <limits>

template<typename _classType>
struct TAsSingleton
{
	using ClassType = _classType;

	static ClassType& Get() noexcept
	{
		static ClassType single;
		return single;
	}
};

template<typename _enumType>
constexpr auto TErrorEnumValue = std::numeric_limits<std::underlying_type_t<_enumType>>::max();

template<typename _enumType>
constexpr inline auto EnumToInt(_enumType _value) noexcept
{
	using intType = std::underlying_type_t<_enumType>;
	return static_cast<intType>(_value);
}

template<typename _type, size_t _arraySize>
constexpr inline auto ArraySize(_type(&)[_arraySize]) noexcept
{
	return _arraySize;
}

template<bool _isUnsigned, size_t _size> using TIntType = typename TemplateUtilDetail::IntTypeHelper<_isUnsigned, _size>::Type;
using TSintPtrType = TIntType<false, sizeof(void*)>;
using TUintPtrType = TIntType<true, sizeof(void*)>;
using TIntPtrType = TUintPtrType;

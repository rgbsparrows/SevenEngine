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

template<typename _type> requires std::is_pointer_v<_type> || std::is_null_pointer_v<_type>
inline auto PointerToInt(_type _pointer)
{
	return reinterpret_cast<uintptr_t>(_pointer);
}

template<typename _type, size_t _arraySize>
constexpr inline auto ArraySize(_type(&)[_arraySize]) noexcept
{
	return _arraySize;
}

template<bool _isUnsigned, size_t _size> using TIntType = typename TemplateUtilDetail::IntTypeHelper<_isUnsigned, _size>::Type;
template<typename _type> using TNumericInfo = typename TemplateUtilDetail::NumericInfo<_type>;
template<size_t _elementSize, size_t _aligned> using TUnconstructAlignedElement = typename TemplateUtilDetail::UnconstructAlignedElement<_elementSize, _aligned>;

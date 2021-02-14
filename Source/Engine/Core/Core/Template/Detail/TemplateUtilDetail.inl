#pragma once

#include <stdint.h>
#include <type_traits>

namespace TemplateUtilDetail
{
	template<bool _isUnsigned, size_t _size> struct IntTypeHelper {};

	template<> struct IntTypeHelper<false, 1> { using Type = int8_t  ; };
	template<> struct IntTypeHelper<false, 2> { using Type = int16_t ; };
	template<> struct IntTypeHelper<false, 4> { using Type = int32_t ; };
	template<> struct IntTypeHelper<false, 8> { using Type = int64_t ; };
	template<> struct IntTypeHelper<true , 1> { using Type = uint8_t ; };
	template<> struct IntTypeHelper<true , 2> { using Type = uint16_t; };
	template<> struct IntTypeHelper<true , 4> { using Type = uint32_t; };
	template<> struct IntTypeHelper<true , 8> { using Type = uint64_t; };

	template<typename _type> requires std::is_integral_v<_type> || std::is_floating_point_v<_type>
	struct NumericInfo
	{
		using Type = _type;

		static constexpr bool IsIntegral = std::is_integral_v<Type>;
		static constexpr bool IsUnsigned = std::is_unsigned_v<Type>;
		static constexpr bool IsFloatingPoint = std::is_floating_point_v<Type>;
		static constexpr size_t NumByte = sizeof(Type);
		static constexpr size_t NumBit = NumByte * 8;
	};

	template<size_t _elementSize, size_t _aligned>
	struct UnconstructAlignedElement
	{
	private:
		IntTypeHelper<true, _aligned> Buffer[(_elementSize + _aligned - 1) / _aligned];
	};
}

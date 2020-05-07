#pragma once

#include <stdint.h>

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
}

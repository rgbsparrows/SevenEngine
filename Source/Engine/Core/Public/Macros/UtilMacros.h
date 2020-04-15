#pragma once

#include <type_traits>

#define REGIST_ARITHMETIC_OPERATOR_FOR_ENUM(_enumType, _operator)							\
constexpr inline _enumType operator _operator(_enumType _left, _enumType _right) noexcept	\
{																							\
	using intType = std::underlying_type_t<_enumType>;										\
	return static_cast<_enumType>(															\
		static_cast<intType>(_left) _operator												\
		static_cast<intType>(_right)														\
	);																						\
}

#define REGIST_ASSIGNMENT_OPERATOR_FOR_ENUM(_enumType, _baseOperator)								\
constexpr inline _enumType operator _baseOperator##=(_enumType & _left, _enumType _right) noexcept	\
{																									\
	return _left = _left _baseOperator _right;														\
}

#define REGIST_ENUM_FLAG(_enumType)\
REGIST_ARITHMETIC_OPERATOR_FOR_ENUM(_enumType, |)\
REGIST_ARITHMETIC_OPERATOR_FOR_ENUM(_enumType, &)\
REGIST_ARITHMETIC_OPERATOR_FOR_ENUM(_enumType, ^)\
REGIST_ASSIGNMENT_OPERATOR_FOR_ENUM(_enumType, |)\
REGIST_ASSIGNMENT_OPERATOR_FOR_ENUM(_enumType, &)\
REGIST_ASSIGNMENT_OPERATOR_FOR_ENUM(_enumType, ^)

#define DEPRECATED(_message) [[deprecated(_message)]]
#define NO_DISCARD_RETURN [[nodiscard]]
#define NORETURN [[noreturn]]

#define COMMENT(...)

#define __MAKE_WIDE(quote) L##quote   
#define MAKE_WIDE(quote) __MAKE_WIDE(quote)

#define __MAKE_STR(quote) #quote
#define MAKE_STR(quote) __MAKE_STR(quote)

#define FILE_LOC __FILE__  "(" MAKE_STR(__LINE__) "): "

#define COMPILE_MESSAGE(messageText) __pragma(message(FILE_LOC messageText))

#define TODO(message) COMPILE_MESSAGE("TODO: " message)

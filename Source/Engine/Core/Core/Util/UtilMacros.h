#pragma once

#include <type_traits>
#include <initializer_list>

#define __REGIST_ARITHMETIC_OPERATOR_FOR_ENUM(_enumType, _operator)							\
constexpr inline _enumType operator _operator(_enumType _left, _enumType _right) noexcept	\
{																							\
	using intType = std::underlying_type_t<_enumType>;										\
	return static_cast<_enumType>(															\
		static_cast<intType>(_left) _operator												\
		static_cast<intType>(_right)														\
	);																						\
}

#define __REGIST_ASSIGNMENT_OPERATOR_FOR_ENUM(_enumType, _baseOperator)								\
constexpr inline _enumType operator _baseOperator##=(_enumType & _left, _enumType _right) noexcept	\
{																									\
	return _left = _left _baseOperator _right;														\
}

#define __REGIST_IS_SET_FUNC_FOR_ENUM(_enumFlagType)												\
constexpr inline bool IsEnumFlagSet(_enumFlagType _enumFlag, _enumFlagType _enumFlagSet) noexcept	\
{																									\
	return (_enumFlag & _enumFlagSet) == _enumFlagSet;												\
}																									\
																									\
constexpr inline bool IsEnumFlagSet(_enumFlagType _enumFlag) noexcept								\
{																									\
	return EnumToInt(_enumFlag) != 0;																\
}																									\

#define __REGIST_ENUM_FLAG_CONVERT_FUNC(_enumFlagType, _baseEnumType)									\
constexpr inline auto ConvertToEnumFlag(_baseEnumType _baseEnum) noexcept								\
{																										\
	using intType = std::underlying_type_t<_baseEnumType>;												\
	return static_cast<_enumFlagType>(static_cast<intType>(1) << static_cast<intType>(_baseEnum));		\
}																										\
																										\
constexpr inline auto ConvertToEnumFlag(std::initializer_list<_baseEnumType> _baseEnumList) noexcept	\
{																										\
	_enumFlagType enumFlag = static_cast<_enumFlagType>(0);												\
	for(_baseEnumType _baseEnum : _baseEnumList)														\
		enumFlag |= ConvertToEnumFlag(_baseEnum);														\
	return enumFlag;																					\
}


#define __REGIST_IS_SET_FUNC_FOR_ENUM_FLAG(_enumFlagType, _baseEnumType)								\
constexpr inline bool IsEnumFlagSet(_enumFlagType _enumFlag, _baseEnumType _baseEnumFlagSet) noexcept	\
{																										\
	return IsEnumFlagSet(_enumFlag, ConvertToEnumFlag(_baseEnumFlagSet));								\
}																										\


#define REGIST_ENUM_FLAG(_enumType)\
__REGIST_ARITHMETIC_OPERATOR_FOR_ENUM(_enumType, |)\
__REGIST_ARITHMETIC_OPERATOR_FOR_ENUM(_enumType, &)\
__REGIST_ARITHMETIC_OPERATOR_FOR_ENUM(_enumType, ^)\
__REGIST_ASSIGNMENT_OPERATOR_FOR_ENUM(_enumType, |)\
__REGIST_ASSIGNMENT_OPERATOR_FOR_ENUM(_enumType, &)\
__REGIST_ASSIGNMENT_OPERATOR_FOR_ENUM(_enumType, ^)\
__REGIST_IS_SET_FUNC_FOR_ENUM(_enumType)

#define REGIST_ENUM_FLAG_FORM_ENUM(_enumFlagType, _baseEnumType)	\
REGIST_ENUM_FLAG(_enumFlagType)										\
__REGIST_ENUM_FLAG_CONVERT_FUNC(_enumFlagType, _baseEnumType)		\
__REGIST_IS_SET_FUNC_FOR_ENUM_FLAG(_enumFlagType, _baseEnumType)	

#define DECLARE_NAMED_CONCEPT_WITH_MEMBER(_conceptType, _member) template <typename _type> concept _conceptType = requires(_type& _value) { _value._member; };	
#define DECLARE_NAMED_CONCEPT_WITH_MEMBER_FUNC(_conceptType, _memberFunc) template <typename _type> concept _conceptType = requires(_type& _value) { _value._memberFunc(); };	
#define DECLARE_NAMED_CONCEPT_WITH_NOEXCEPT_MEMBER_FUNC(_conceptType, _memberFunc) template <typename _type> concept _conceptType = requires(_type& _value) { { _value._memberFunc() } noexcept; };

#define DECLARE_CONCEPT_WITH_MEMBER(_member) DECLARE_NAMED_CONCEPT_WITH_MEMBER(CTypeWith##_member, _member)
#define DECLARE_CONCEPT_WITH_MEMBER_FUNC(_member) DECLARE_NAMED_CONCEPT_WITH_MEMBER_FUNC(CTypeWith##_member, _member)
#define DECLARE_CONCEPT_WITH_NOEXCEPT_MEMBER_FUNC(_member) DECLARE_NAMED_CONCEPT_WITH_NOEXCEPT_MEMBER_FUNC(CTypeWith##_member, _member)

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

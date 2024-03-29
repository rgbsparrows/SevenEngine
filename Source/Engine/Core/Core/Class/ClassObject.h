﻿#pragma once

#include "Core/Util/NameOf.h"
#include "Core/Util/TemplateUtil.h"

#include <string>
#include <stdint.h>

__interface IClassObjectInterface;

using SClassIdentifier = THandleType<CalcStrHash("SClassIdentifier")>;

namespace ClassObjectDetail
{
	void RegistClassObject(SClassIdentifier _typeHash, const IClassObjectInterface* _classObject) noexcept;
}

__interface IClassObjectInterface
{
	SClassIdentifier GetClassHash()const noexcept;
	std::string_view GetClassFullName()const noexcept;
	std::string_view GetClassShortName()const noexcept;
	size_t GetClassSize()const noexcept;
	size_t GetClassAlign()const noexcept;
	bool IsAbstractClass()const noexcept;
	bool IsFinalClass()const noexcept;
	bool IsInstanceOf(SClassIdentifier _classHash)const noexcept;
	bool IsImplementedFrom(SClassIdentifier _interfaceHash)const noexcept;
	bool IsDrivedFrom(SClassIdentifier _classHash)const noexcept;
	bool IsDrivedFromAncestor(SClassIdentifier _ancestorHash)const noexcept;
};

template<typename _ancestorClass>
struct TClassObjectInterface : IClassObjectInterface
{
	using AncestorClass = _ancestorClass;

	static constexpr bool StaticIsDrivedFromAncestor(SClassIdentifier _ancestorHash)noexcept { return TTypeHash<AncestorClass> == _ancestorHash; }

	bool IsDrivedFromAncestor(SClassIdentifier _ancestorHash)const noexcept override { return StaticIsDrivedFromAncestor(_ancestorHash); }
	virtual AncestorClass* ConstructObject()const noexcept = 0;
};

template<typename _type, typename _baseClass, typename... _interface>
struct TClassObject final : _type::ClassObjectInterfaceType
{
public:
	using Type = _type;
	using BaseClass = _baseClass;
	using AncestorClass = typename Type::ClassObjectInterfaceType::AncestorClass;

	constexpr static bool IsAncestorClass = std::is_void_v<BaseClass>;

	static constexpr SClassIdentifier StaticGetClassHash() noexcept { return TTypeHash<Type>; }
	static constexpr std::string_view StaticGetClassFullName() noexcept { return TFullTypeName<Type>; }
	static constexpr std::string_view StaticGetClassShortName() noexcept { return TShortTypeName<Type>; }
	static constexpr size_t StaticGetClassSize() noexcept { return sizeof(Type); }
	static constexpr size_t StaticGetClassAlign() noexcept { return alignof(Type); }
	static constexpr bool StaticIsAbstractClass() noexcept { return std::is_abstract_v<Type>; }
	static constexpr bool StaticIsFinalClass() noexcept { return std::is_final_v<Type>; }
	static constexpr bool StaticIsInstanceOf(SClassIdentifier _classHash) noexcept
	{
		return StaticGetClassHash() == _classHash || StaticIsImplementedFrom(_classHash) || StaticIsDrivedFrom(_classHash);
	}
	static constexpr bool StaticIsImplementedFrom(SClassIdentifier _interfaceHash) noexcept
	{
		if constexpr (IsAncestorClass) return ((TTypeHash<_interface> == _interfaceHash) || ...);
		else return ((TTypeHash<_interface> == _interfaceHash) || ...) || BaseClass::ClassObjectType::StaticIsImplementedFrom(_interfaceHash);
	}
	static constexpr bool StaticIsDrivedFrom(SClassIdentifier _classHash)
	{
		if constexpr (std::is_void_v<BaseClass>) return false;
		else return TTypeHash<BaseClass> == _classHash || BaseClass::ClassObjectType::StaticIsDrivedFrom(_classHash);
	}
	template<typename... _argts> static Type* StaticConstructObject(_argts... _argvs) noexcept
	{
		if constexpr (std::is_abstract_v<Type>) return nullptr;
		else return new Type(_argvs...);
	}

	TClassObject() { ClassObjectDetail::RegistClassObject(StaticGetClassHash(), this); }

	SClassIdentifier GetClassHash()const noexcept override { return StaticGetClassHash(); }
	std::string_view GetClassFullName()const noexcept override { return StaticGetClassFullName(); }
	std::string_view GetClassShortName()const noexcept override { return StaticGetClassShortName(); }
	size_t GetClassSize()const noexcept override { return StaticGetClassSize(); }
	size_t GetClassAlign()const noexcept override { return StaticGetClassAlign(); }
	bool IsAbstractClass()const noexcept override { return StaticIsAbstractClass(); }
	bool IsFinalClass()const noexcept override { return StaticIsFinalClass(); }
	bool IsInstanceOf(SClassIdentifier _classHash)const noexcept override { return StaticIsInstanceOf(_classHash); }
	bool IsImplementedFrom(SClassIdentifier _interfaceHash)const noexcept override { return StaticIsImplementedFrom(_interfaceHash); }
	bool IsDrivedFrom(SClassIdentifier _classHash)const noexcept override { return StaticIsDrivedFrom(_classHash); }
	Type* ConstructObject()const noexcept override { return StaticConstructObject(); }
};

//DECLARE_ANCESTOR_CLASSOBJECT_BODY(_ancestorClass)
#define DECLARE_ANCESTOR_CLASSOBJECT_BODY(_class)																\
public:																											\
	using ClassObjectInterfaceType = TClassObjectInterface<_class>;												\
	using ClassObjectType = TClassObject<_class, void>;															\
	static const ClassObjectType* StaticGetClassObject() noexcept { return &ClassObject; }						\
	virtual const ClassObjectInterfaceType* GetClassObject()const noexcept { return StaticGetClassObject(); }	\
	using AncestorClass = typename ClassObjectType::AncestorClass;												\
private:																											\
	inline static ClassObjectType ClassObject;																	

//DECLARE_CLASSOBJECT_BODY(_class, _baseClass, _interface)
#define DECLARE_CLASSOBJECT_BODY(_class, _baseClass, ...)														\
public:																											\
	using ClassObjectType = TClassObject<_class, _baseClass, __VA_ARGS__>;										\
	static const ClassObjectType* StaticGetClassObject() noexcept { return &ClassObject; }						\
	const ClassObjectInterfaceType* GetClassObject()const noexcept override { return StaticGetClassObject(); }	\
	using AncestorClass = typename ClassObjectType::AncestorClass;												\
	using BaseClass = typename ClassObjectType::BaseClass;														\
private:																											\
	inline static ClassObjectType ClassObject;																	

template<typename _type>
constexpr inline SClassIdentifier GetClassHash()
{
	return _type::StaticGetClassObject()->StaticGetClassHash();
}

template<typename _type>
inline SClassIdentifier GetClassHash(const _type* _object)
{
	return _object->GetClassObject()->GetClassHash();
}

template<typename _baseType, typename _type>
inline bool IsInstanceOf(const _type* _object)
{
	return _object->GetClassObject()->IsInstanceOf(_baseType::StaticGetClassObject()->GetClassHash());
}

template<typename _type>
inline bool IsInstanceOf(const _type* _object, SClassIdentifier _baseClassHash)
{
	return _object->GetClassObject()->IsInstanceOf(_baseClassHash);
}

template<typename _interfaceType, typename _type>
inline bool IsImplementedFrom(const _type* _object)
{
	return _object->GetClassObject()->IsImplementedFrom(TTypeHash<_interfaceType>);
}

template<typename _baseType, typename _type> requires requires { _baseType::StaticGetClassObject(); }
inline bool IsDrivedFrom(const _type* _object)
{
	return _object->GetClassObject()->IsDrivedFrom(_baseType::StaticGetClassObject()->GetClassHash());
}

template<typename _type>
inline bool IsDrivedFrom(const _type* _object, SClassIdentifier _baseClassHash)
{
	return _object->GetClassObject()->IsDrivedFrom(_baseClassHash);
}

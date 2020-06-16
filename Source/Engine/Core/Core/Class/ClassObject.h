#pragma once

#include "Core/Template/NameOf.h"
#include "Core/Macros/ConditionDeprecated.h"

#include <string>
#include <stdint.h>

__interface IClassObjectInterface;

namespace ClassObjectDetail
{
	void RegistClassObject(uint64_t _typeHash, const IClassObjectInterface* _classObject) noexcept;
}

__interface IClassObjectInterface
{
	uint64_t GetClassHash()const noexcept;
	std::wstring_view GetClassFullName()const noexcept;
	std::wstring_view GetClassShortName()const noexcept;
	size_t GetClassSize()const noexcept;
	size_t GetClassAlign()const noexcept;
	bool IsAbstractClass()const noexcept;
	bool IsFinalClass()const noexcept;
	bool IsInstanceOf(uint64_t _classHash)const noexcept;
	bool IsImplementedFrom(uint64_t _interfaceHash)const noexcept;
	bool IsDrivedFrom(uint64_t _classHash)const noexcept;
	bool IsDrivedFromAncestor(uint64_t _ancestorHash)const noexcept;
};

template<typename _ancestorClass>
struct TClassObjectInterface : IClassObjectInterface
{
	using AncestorClass = _ancestorClass;

	static constexpr bool StaticIsDrivedFromAncestor(uint64_t _ancestorHash)noexcept { return TTypeHash<AncestorClass> == _ancestorHash; }

	bool IsDrivedFromAncestor(uint64_t _ancestorHash)const noexcept override { return StaticIsDrivedFromAncestor(_ancestorHash); }
	virtual AncestorClass* ConstructObject(void* _objectBuffer)const noexcept = 0;
};

REWRITE_WHEN_CONCEPT_AVAILABLE(
	"借助concept完成对type和baseclass模板参数的约束，其中应该包含如下约束\n"
	"	* _type, _baseClass是否均继承自UObject\n"
	"	* _type是否继承自_baseClass(除非_baseClass是void，这时表示没有基类(UObject))\n"
	"	* _type继承自_interface\n"
	"	* _interface是否都是抽象的，且没有数据成员(is_abstract_v<_interface> && sizeof(_interface) == sizeof(void*))\n"
	"	* _type是否具有默认构造函数\n"
	"	* _type能访问到ClassObjectInterfaceType类型(在祖先类中被定义)\n"
)
template<typename _type, typename _baseClass, typename... _interface>
struct TClassObject final : _type::ClassObjectInterfaceType
{
public:
	using Type = _type;
	using BaseClass = _baseClass;
	using AncestorClass = typename Type::ClassObjectInterfaceType::AncestorClass;

	constexpr static bool IsAncestorClass = std::is_void_v<BaseClass>;

	static constexpr uint64_t StaticGetClassHash() noexcept { return TTypeHash<Type>; }
	static constexpr std::wstring_view StaticGetClassFullName() noexcept { return TFullTypeName<Type>; }
	static constexpr std::wstring_view StaticGetClassShortName() noexcept { return TShortTypeName<Type>; }
	static constexpr size_t StaticGetClassSize() noexcept { return sizeof(Type); }
	static constexpr size_t StaticGetClassAlign() noexcept { return alignof(Type); }
	static constexpr bool StaticIsAbstractClass() noexcept { return std::is_abstract_v<Type>; }
	static constexpr bool StaticIsFinalClass() noexcept { return std::is_final_v<Type>; }
	static constexpr bool StaticIsInstanceOf(uint64_t _classHash) noexcept
	{
		return StaticGetClassHash() == _classHash || StaticIsImplementedFrom(_classHash) || StaticIsDrivedFrom(_classHash);
	}
	static constexpr bool StaticIsImplementedFrom(uint64_t _interfaceHash) noexcept
	{
		if constexpr (IsAncestorClass) return ((TTypeHash<_interface> == _interfaceHash) || ...);
		else return ((TTypeHash<_interface> == _interfaceHash) || ...) || BaseClass::ClassObjectType::StaticIsImplementedFrom(_interfaceHash);
	}
	static constexpr bool StaticIsDrivedFrom(uint64_t _classHash)
	{
		if constexpr (std::is_void_v<BaseClass>) return false;
		else return TTypeHash<BaseClass> == _classHash || BaseClass::ClassObjectType::StaticIsDrivedFrom(_classHash);
	}
	template<typename... _argts> static Type* StaticConstructObject(void* _objectBuffer, _argts... _argvs) noexcept
	{
		if constexpr (std::is_abstract_v<Type>) return nullptr;
		else return new(_objectBuffer) Type(_argvs...);
	}

	TClassObject() { ClassObjectDetail::RegistClassObject(StaticGetClassHash(), this); }

	uint64_t GetClassHash()const noexcept override { return StaticGetClassHash(); }
	std::wstring_view GetClassFullName()const noexcept override { return StaticGetClassFullName(); }
	std::wstring_view GetClassShortName()const noexcept override { return StaticGetClassShortName(); }
	size_t GetClassSize()const noexcept override { return StaticGetClassSize(); }
	size_t GetClassAlign()const noexcept override { return StaticGetClassAlign(); }
	bool IsAbstractClass()const noexcept override { return StaticIsAbstractClass(); }
	bool IsFinalClass()const noexcept override { return StaticIsFinalClass(); }
	bool IsInstanceOf(uint64_t _classHash)const noexcept override { return StaticIsInstanceOf(_classHash); }
	bool IsImplementedFrom(uint64_t _interfaceHash)const noexcept override { return StaticIsImplementedFrom(_interfaceHash); }
	bool IsDrivedFrom(uint64_t _classHash)const noexcept override { return StaticIsDrivedFrom(_classHash); }
	Type* ConstructObject(void* _objectBuffer)const noexcept override { return StaticConstructObject(_objectBuffer); }
};

//DECLARE_ANCESTOR_CLASSOBJECT_BODY(_ancestorClass)
#define DECLARE_ANCESTOR_CLASSOBJECT_BODY(_class)																\
public:																											\
	using ClassObjectInterfaceType = TClassObjectInterface<_class>;												\
	using ClassObjectType = TClassObject<_class, void>;															\
	static const ClassObjectType* StaticGetClassObject() noexcept { return &ClassObject; }						\
	virtual const ClassObjectInterfaceType* GetClassObject()const noexcept { return StaticGetClassObject(); }	\
private:																										\
	inline static ClassObjectType ClassObject;																	

//DECLARE_CLASSOBJECT_BODY(_class, _baseClass, _interface)
#define DECLARE_CLASSOBJECT_BODY(_class, ...)																	\
public:																											\
	using ClassObjectType = TClassObject<_class, __VA_ARGS__>;													\
	static const ClassObjectType* StaticGetClassObject() noexcept { return &ClassObject; }						\
	const ClassObjectInterfaceType* GetClassObject()const noexcept override { return StaticGetClassObject(); }	\
	using AncestorClass = typename ClassObjectType::AncestorClass;													\
	using BaseClass = typename ClassObjectType::BaseClass;															\
private:																										\
	inline static ClassObjectType ClassObject;																	

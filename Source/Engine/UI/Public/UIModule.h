#pragma once

#include "UIObject.h"
#include "Class/ClassObject.h"
#include "Class/ClassManager.h"
#include "Modules/ModuleInterface.h"

using IUIClassObjectInterface = TClassObjectInterface<SUIObject>;

class IUIModuleInterface : public IModuleInterface
{
public:
	virtual void ProcessMessage() noexcept = 0;

	virtual SUIObject* CreateUIObject(uint64_t _typeHash) noexcept = 0;
	virtual SUIObject* CreateUIObject(const IClassObjectInterface* _classObject) noexcept = 0;
	virtual SUIObject* CreateUIObject(const IUIClassObjectInterface* _classObject) noexcept = 0;

	template<typename _type>
	_type* CreateUIObject(uint64_t _typeHash) noexcept
	{
		using Type = _type;
		auto classObject = SClassManager::Get().GetClassObject(_typeHash);
		if (classObject) return CreateUIObject<Type>(classObject);
		else return nullptr;
	}

	template<typename _type>
	_type* CreateUIObject(const IClassObjectInterface* _classObject) noexcept
	{
		using Type = _type;
		if (_classObject->IsDrivedFromAncestor(SUIObject::ClassObjectType::StaticGetClassHash()))
			return CreateUIObject<Type>(static_cast<const IUIClassObjectInterface*>(_classObject))
		else return nullptr;
	}

	template<typename _type>
	_type* CreateUIObject(const IUIClassObjectInterface* _classObject) noexcept
	{
		using Type = _type;
		if (_classObject->IsInstanceOf(TTypeHash<Type>))
			return static_cast<Type*>(CreateUIObject(_classObject));
		else return nullptr;
	}
};

IUIModuleInterface* GetUIModule() noexcept;

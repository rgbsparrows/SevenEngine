#pragma once

#include "Core/Util/Assert.h"
#include "Core/Util/TemplateUtil.h"
#include "Core/Class/ClassObject.h"

#include <map>

__interface IClassObjectInterface;

class SClassManager final : public TAsSingleton<SClassManager>
{
public:
	void RegistClassObject(SClassIdentifier _classHash, const IClassObjectInterface* _classObject) noexcept
	{
		ASSERT(_classHash != 0 && _classObject!= nullptr);
		ASSERT(mClassObjectMap.count(_classHash) == 0);

		mClassObjectMap[_classHash] = _classObject;
	}

	void Init() noexcept {}
	void Clear() noexcept {}

	const IClassObjectInterface* GetClassObject(SClassIdentifier _classHash)const noexcept
	{
		auto it = mClassObjectMap.find(_classHash);

		if (it == mClassObjectMap.end()) return nullptr;
		else return it->second;
	}

	std::vector<SClassIdentifier> GetAllDrivedClass(SClassIdentifier _baseClass) const noexcept
	{
		std::vector<SClassIdentifier> result;

		for (auto& [classHash, classObject] : mClassObjectMap)
		{
			if (classObject->IsDrivedFrom(_baseClass))
			{
				result.push_back(classHash);
			}
		}

		return result;
	}

	template<typename _classType>
	_classType* ConstructObject(SClassIdentifier _classHash)
	{
		using AncestorClassType = _classType::AncestorClass;
		using ClassType = _classType;

		const IClassObjectInterface* classObject = GetClassObject(_classHash);
		if (classObject->IsInstanceOf(TTypeHash<ClassType>) == false)
			return nullptr;

		AncestorClassType* object = static_cast<const TClassObjectInterface<AncestorClassType>*>(classObject)->ConstructObject();
		return static_cast<ClassType*>(object);
	}

private:
	std::map<SClassIdentifier, const IClassObjectInterface*> mClassObjectMap;
};

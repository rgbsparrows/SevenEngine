#pragma once

#include "Core/Macros/Assert.h"
#include "Core/Template/TemplateUtil.h"

#include <map>

__interface IClassObjectInterface;

class SClassManager final : public TAsSingleton<SClassManager>
{
public:
	void RegistClassObject(uint64_t _classHash, const IClassObjectInterface* _classObject) noexcept
	{
		ASSERT(_classHash != 0 && _classObject!= nullptr);
		ASSERT(mClassObjectMap.count(_classHash) == 0);

		mClassObjectMap[_classHash] = _classObject;
	}

	void Init() noexcept {}
	void Clear() noexcept {}

	const IClassObjectInterface* GetClassObject(uint64_t _classHash)const noexcept
	{
		auto it = mClassObjectMap.find(_classHash);

		if (it == mClassObjectMap.end()) return nullptr;
		else return it->second;
	}

private:
	std::map<uint64_t, const IClassObjectInterface*> mClassObjectMap;
};

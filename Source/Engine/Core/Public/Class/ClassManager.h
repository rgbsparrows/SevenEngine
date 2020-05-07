#pragma once

#include "Container/Contanier.h"
#include "Template/TemplateUtil.h"

__interface IClassObjectInterface;

class SClassManager final : public TAsSingleton<SClassManager>
{
public:
	void RegistClassObject(uint64_t _classHash, IClassObjectInterface* _classObject) noexcept
	{
		mClassObjects.push_back(std::make_tuple(_classHash, _classObject));
	}

	void Init() noexcept;
	void Clear() noexcept;

	IClassObjectInterface* GetClassObject(uint64_t _classHash)const noexcept
	{
		for (auto& _ele : mClassObjects)
			if (std::get<0>(_ele) == _classHash)
				return std::get<1>(_ele);

		return nullptr;
	}

private:
	PackageVector<uint64_t, IClassObjectInterface*> mClassObjects;
};

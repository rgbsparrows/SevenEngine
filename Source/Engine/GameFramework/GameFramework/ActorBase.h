#pragma once

#include "Core/Class/ClassObject.h"
#include "GameFramework/GameObject.h"

#include <vector>

namespace GameFramework
{
	class SComponentBase;

	class SActorBase : public SGameObject
	{
		DECLARE_CLASSOBJECT_BODY(SActorBase, SGameObject)

	public:
		std::vector<SComponentBase*> GetComponents() const noexcept { return mComponents; }

	private:
		std::wstring mActorName;
		std::vector<SComponentBase*> mComponents;
	};
}

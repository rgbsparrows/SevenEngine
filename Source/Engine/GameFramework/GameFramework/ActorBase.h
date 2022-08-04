#pragma once

#include "Core/Class/ClassObject.h"
#include "GameFramework/GameObject.h"
#include "GameFramework/Transform.h"

#include <vector>

namespace GameFramework
{
	class SComponentBase;

	class SActorBase : public SGameObject
	{
		DECLARE_CLASSOBJECT_BODY(SActorBase, SGameObject)

	public:
		void SetActorName(const std::wstring& _actorName) { mActorName = _actorName; }
		const std::wstring& GetActorName()const { return mActorName; }

		std::vector<SComponentBase*> GetComponents() const noexcept { return mComponents; }

	private:
		std::wstring mActorName;
		SActorBase* mParent = nullptr;
		std::vector<SActorBase*> mChildren;
		std::vector<SComponentBase*> mComponents;

		Transform mTransform;
	};
}

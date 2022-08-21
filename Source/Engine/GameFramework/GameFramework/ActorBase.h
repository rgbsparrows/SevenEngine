#pragma once

#include "Core/Class/ClassObject.h"
#include "GameFramework/GameObject.h"
#include "GameFramework/Transform.h"

#include <vector>

namespace GameFramework
{
	class UComponentBase;

	class UActorBase : public UGameObject
	{
		DECLARE_CLASSOBJECT_BODY(UActorBase, UGameObject)

	public:
		void SetActorName(const std::wstring& _actorName) { mActorName = _actorName; }
		const std::wstring& GetActorName()const { return mActorName; }

		std::vector<UComponentBase*> GetComponents() const noexcept { return mComponents; }

	private:
		std::wstring mActorName;
		std::vector<UComponentBase*> mComponents;

		STransform mTransform;
	};
}

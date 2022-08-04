#pragma once

#include "Core/Class/ClassObject.h"
#include "GameFramework/GameObject.h"
#include "GameFramework/Transform.h"

namespace GameFramework
{
	class SActorBase;

	class SComponentBase : public SGameObject
	{
		DECLARE_CLASSOBJECT_BODY(SComponentBase, SGameObject)

	public:

	private:
		SActorBase* mActor = nullptr;

		Transform mTransform;
	};
}

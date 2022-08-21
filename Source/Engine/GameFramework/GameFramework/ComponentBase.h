#pragma once

#include "Core/Class/ClassObject.h"
#include "GameFramework/GameObject.h"
#include "GameFramework/Transform.h"

namespace GameFramework
{
	class UActorBase;

	class UComponentBase : public UGameObject
	{
		DECLARE_CLASSOBJECT_BODY(UComponentBase, UGameObject)

	public:

	private:
		UActorBase* mActor = nullptr;

		STransform mTransform;
	};
}

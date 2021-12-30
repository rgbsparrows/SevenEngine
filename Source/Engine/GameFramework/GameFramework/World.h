#pragma once

#include "GameFramework/GameObject.h"

#include <vector>

namespace GameFramework
{
	class SActorBase;

	class SWorldBase : public SGameObject
	{
		DECLARE_CLASSOBJECT_BODY(SWorldBase, SGameObject)

	public:
		const std::vector<SActorBase*>& GetActorList() const noexcept { return mActorList; }

	private:
		void RefrashRenderProxy() noexcept;

	private:
		std::vector<SActorBase*> mActorList;
	};
}

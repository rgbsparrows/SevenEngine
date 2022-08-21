#pragma once

#include "GameFramework/GameObject.h"

#include <vector>
#include "Render/RenderProxy/World/WorldProxy.h"

namespace GameFramework
{
	class UActorBase;

	class UWorldBase : public UGameObject
	{
		DECLARE_CLASSOBJECT_BODY(UWorldBase, UGameObject)

	public:
		void Init() noexcept;
		void Tick() noexcept;

	public:
		const std::vector<UActorBase*>& GetActorList() const noexcept { return mActorList; }
		RRenderProxy<R3DWorld>* GetWorldRenderProxy() noexcept { return mWorldRenderProxy; }

	public:
		void RefrashRenderProxy() noexcept;

	private:
		std::vector<UActorBase*> mActorList;
		RRenderProxy<R3DWorld>* mWorldRenderProxy = nullptr;
	};
}

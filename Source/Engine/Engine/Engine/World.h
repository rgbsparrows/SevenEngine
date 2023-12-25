#pragma once

#include "Core/Class/ClassObject.h"
#include "Engine/RenderScene.h"

#include <vector>
#include <concepts>

class AActor;
class ACComponent;

class SWorld
{
public:
	static SWorld* MakeNewWorld() noexcept;

	void Init() noexcept;
	void Release() noexcept;

	void Tick() noexcept;

	template<std::derived_from<AActor> _actorType>
	auto SpawnActor() noexcept
	{
		using ActorType = _actorType;
		return static_cast<ActorType*>(SpawnActor(GetClassHash<ActorType>()));
	}

	AActor* SpawnActor(SClassIdentifier _actorClass) noexcept;

	std::vector<AActor*> GetActorList() noexcept { return mActorList; }

	RRenderProxy<RWorld>* GetRenderProxy() noexcept { return mRenderScene.GetRenderProxy(); }

public:
	void RegisterActor_Internal(AActor* _actor) noexcept;
	void UnregisterActor_Internal(AActor* _actor) noexcept;

	void RegisterComponent_Internal(ACComponent* _component) noexcept;
	void UnregisterComponent_Internal(ACComponent* _component) noexcept;

private:
	std::vector<AActor*> mActorList;
	SRenderScene mRenderScene;
};

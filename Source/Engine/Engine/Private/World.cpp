#include "Engine/World.h"
#include "Core/Util/Algorithm.h"
#include "Engine/Classes/Actor.h"
#include "Core/Class/ClassManager.h"

SWorld* SWorld::MakeNewWorld() noexcept
{
	SWorld* world = new SWorld;
	world->Init();

	return world;
}

void SWorld::Init() noexcept
{
	TODO("World Init");

	mRenderScene.Init(this);
}

void SWorld::Release() noexcept
{
	mRenderScene.Clear();

	TODO("World Clear");
}

void SWorld::Tick() noexcept
{
	TODO("World Tick");

	mRenderScene.UpdateRenderProxy(); 

	for (AActor* actor : mActorList)
		actor->LateUpdateRenderProxy_Internal();
}

AActor* SWorld::SpawnActor(SClassIdentifier _actorClass) noexcept
{
	if (AActor* newActor = SClassManager::Get().ConstructObject<AActor>(_actorClass))
		newActor->Init(this);

	return nullptr;
}

void SWorld::RegisterActor_Internal(AActor* _actor) noexcept
{
	CHECK(_actor);
	mActorList.push_back(_actor);
}

void SWorld::UnregisterActor_Internal(AActor* _actor) noexcept
{
	CHECK(_actor);
	EraseFirst(mActorList, _actor);
}

void SWorld::RegisterComponent_Internal(ACComponent* _component) noexcept
{
	if(IsInstanceOf<ACRenderComponent>(_component))
		mRenderScene.RegistComponent(static_cast<ACRenderComponent*>(_component));
}

void SWorld::UnregisterComponent_Internal(ACComponent* _component) noexcept
{
	if (IsInstanceOf<ACRenderComponent>(_component))
		mRenderScene.UnregistComponent(static_cast<ACRenderComponent*>(_component));
}

#include "Engine/World.h"
#include "Core/Util/Assert.h"
#include "Core/Util/Algorithm.h"
#include "Engine/Classes/Actor.h"
#include "Core/Class/ClassManager.h"
#include "Engine/Classes/Component.h"

void AActor::Init(SWorld* _world) noexcept
{
	mWorld = _world;

	GetWorld()->RegisterActor_Internal(this);
}

void AActor::Release() noexcept
{
	GetWorld()->UnregisterActor_Internal(this);

	delete this;
}

ACComponent* AActor::SpawnComponent(ACComponent* _parent, SClassIdentifier _componentClass) noexcept
{
	CHECK(_parent == nullptr || _parent->GetActor() == this);

	ACComponent* component = SClassManager::Get().ConstructObject<ACComponent>(_componentClass);
	if (component)
	{
		if (_parent)
			_parent->AddChild_Internal(component);
		else
			AddChild_Internal(component);

		component->Init(this, _parent);
	}

	return component;
}

void AActor::SetTransform(const Math::STransform& _newTransforms) noexcept 
{
	mTransform = _newTransforms;
	MarkRnederProxyDirty_Internal();
}

void AActor::AddChild_Internal(ACComponent* _childComponent, size_t _index) noexcept
{
	CHECK(_index == SIZE_MAX || _index <= mChildren.size());
	CHECK(Contains(mChildren, _childComponent) == false);

	mChildren.insert(std::next(mChildren.begin(), _index == SIZE_MAX ? mChildren.size() : _index), _childComponent);
}

void AActor::RemoveChild_Internal(ACComponent* _childComponent) noexcept
{
	CHECK(Contains(mChildren, _childComponent));

	EraseFirst(mChildren, _childComponent);
}

void AActor::MarkRnederProxyDirty_Internal() noexcept
{
	for (ACComponent* component : mChildren)
		component->MarkRnederProxyDirty_Internal();
}

void AActor::LateUpdateRenderProxy_Internal() noexcept
{
	for (ACComponent* component : mChildren)
		component->LateUpdateRenderProxy_Internal();
}

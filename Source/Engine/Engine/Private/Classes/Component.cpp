#include "Engine/World.h"
#include "Core/Math/Math.h"
#include "Core/Util/Assert.h"
#include "Core/Util/Algorithm.h"
#include "Engine/Classes/Actor.h"
#include "Engine/Classes/Component.h"
#include "Core/Class/ClassObject.h"
#include "Engine/Classes/RenderComponent.h"

void ACComponent::Init(AActor* _actor, ACComponent* _parent) noexcept
{
	mActor = _actor;
	mParent = _parent;

	GetWorld()->RegisterComponent_Internal(this);
}

void ACComponent::Release() noexcept
{
	GetWorld()->UnregisterComponent_Internal(this);

	delete this;
}

void ACComponent::ResetParent(ACComponent* _parent) noexcept
{
	CHECK(_parent == nullptr || _parent->GetActor() == GetActor());
	CHECK(IsParentOf(_parent) == false);

	if (GetParent() == nullptr)
		GetActor()->RemoveChild_Internal(this);
	else
		GetParent()->RemoveChild_Internal(this);

	mParent = _parent;

	if (GetParent() == nullptr)
		GetActor()->AddChild_Internal(this);
	else
		GetParent()->AddChild_Internal(this);
}

SWorld* ACComponent::GetWorld() noexcept 
{
	return GetActor()->GetWorld();
}

bool ACComponent::IsChildrenOf(ACComponent* _parent, bool _recursion) noexcept
{
	if (GetParent() == _parent)
		return true;
	else if (_recursion == true && _parent == nullptr)
		return true;
	else if (_recursion)
	{
		for (ACComponent* parent = GetParent(); parent != nullptr; parent = parent->GetParent())
			if (parent == _parent)
				return true;
	}

	return false;
}

bool ACComponent::IsParentOf(ACComponent* _child, bool _Recursion) noexcept
{
	CHECK(_child != nullptr);

	return _child->IsChildrenOf(this);
}

Math::STransform ACComponent::GetParentTransform() const noexcept
{
	if (GetParent())
		return GetParent()->GetWorldTransform();
	else
		return GetActor()->GetTransform();
}

Math::STransform ACComponent::GetWorldTransform() const noexcept
{
	return Math::ApplyTransform(GetParentTransform(), mTransform);
}

void ACComponent::SetLocalTransform(const Math::STransform& _newTransform) noexcept
{
	mTransform = _newTransform;
	MarkRnederProxyDirty_Internal();
}

void ACComponent::AddChild_Internal(ACComponent* _childComponent, size_t _index) noexcept
{
	CHECK(_index == SIZE_MAX || _index <= mChildren.size());
	CHECK(Contains(mChildren, _childComponent) == false);

	mChildren.insert(std::next(mChildren.begin(), _index == SIZE_MAX ? mChildren.size() : _index), _childComponent);
}

void ACComponent::RemoveChild_Internal(ACComponent* _childComponent) noexcept
{
	CHECK(Contains(mChildren, _childComponent));

	EraseFirst(mChildren, _childComponent);
}

void ACComponent::MarkRnederProxyDirty_Internal() noexcept
{
	for (ACComponent* child : mChildren)
		child->MarkRnederProxyDirty_Internal();

	if(IsDrivedFrom<ACRenderComponent>(this))
		static_cast<ACRenderComponent*>(this)->MarkRenderProxyDirty();
}

void ACComponent::LateUpdateRenderProxy_Internal() noexcept
{
	for (ACComponent* child : mChildren)
		child->LateUpdateRenderProxy_Internal();

	if (IsDrivedFrom<ACRenderComponent>(this))
		static_cast<ACRenderComponent*>(this)->LateUpdateRenderProxy();
}

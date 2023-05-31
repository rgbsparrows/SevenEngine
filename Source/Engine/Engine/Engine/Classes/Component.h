#pragma once

#include "Core/Math/Type.h"
#include "Core/Class/ClassObject.h"

#include <vector>

class AActor;
class SWorld;

class ACComponent
{
	DECLARE_ANCESTOR_CLASSOBJECT_BODY(ACComponent)

public:
	virtual void Init(AActor* _actor, ACComponent* _parent) noexcept;
	virtual void Release() noexcept;

	void ResetParent(ACComponent* _parent) noexcept;

	SWorld* GetWorld() noexcept;
	AActor* GetActor() noexcept { return mActor; }
	const AActor* GetActor() const noexcept { return mActor; }
	ACComponent* GetParent() noexcept { return mParent; }
	const ACComponent* GetParent() const noexcept { return mParent; }

	bool IsChildrenOf(ACComponent* _parent, bool _recursion = true) noexcept;
	bool IsParentOf(ACComponent* _child, bool _Recursion = true) noexcept;

	Math::STransform GetParentTransform() const noexcept;
	Math::STransform GetWorldTransform() const noexcept;
	Math::STransform GetLocalTransform() const noexcept { return mTransform; }
	void SetLocalTransform(const Math::STransform& _newTransform) noexcept;

public:
	void AddChild_Internal(ACComponent* _childComponent, size_t _index = SIZE_MAX) noexcept;
	void RemoveChild_Internal(ACComponent* _childComponent) noexcept;

	void MarkRnederProxyDirty_Internal() noexcept;
	void LateUpdateRenderProxy_Internal() noexcept;

private:
	AActor* mActor;
	ACComponent* mParent;

	Math::STransform mTransform;
	std::vector<ACComponent*> mChildren;
};

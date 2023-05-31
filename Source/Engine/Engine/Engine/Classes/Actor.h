#pragma once

#include "Core/Math/Type.h"
#include "Core/Class/ClassObject.h"

#include <vector>
#include <limits>

class SWorld;
class ACComponent;

class AActor
{
	DECLARE_ANCESTOR_CLASSOBJECT_BODY(AActor)

public:
	virtual void Init(SWorld* _world) noexcept;
	virtual void Release() noexcept;

	SWorld* GetWorld() noexcept { return mWorld; }
	SWorld* GetWorld() const noexcept { return mWorld; }

	template<std::derived_from<ACComponent> _componentType>
	auto SpawnComponent(ACComponent* _parent) noexcept
	{
		using ComponentType = _componentType;
		return static_cast<ComponentType*>(SpawnComponent(GetClassHash<ComponentType>()));
	}

	ACComponent* SpawnComponent(ACComponent* _parent, SClassIdentifier _componentClass) noexcept;

	Math::STransform GetTransform() const noexcept { return mTransform; }
	void SetTransform(const Math::STransform& _newTransforms) noexcept;

public:
	void AddChild_Internal(ACComponent* _childComponent, size_t _index = SIZE_MAX) noexcept;
	void RemoveChild_Internal(ACComponent* _childComponent) noexcept;

	void MarkRnederProxyDirty_Internal() noexcept;
	void LateUpdateRenderProxy_Internal() noexcept;

private:
	SWorld* mWorld;

	Math::STransform mTransform;
	std::vector<ACComponent*> mChildren;
};

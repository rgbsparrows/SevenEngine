#pragma once
#include "Engine/Classes/Actor.h"
#include "Engine/Classes/RenderComponent.h"
#include "Resource/ResourceType/StaticMeshResource.h"

struct RStaticMeshProxy;

class ACStaticMesh : public ACRenderComponent
{
	DECLARE_CLASSOBJECT_BODY(ACStaticMesh, ACRenderComponent)

public:
	void SetStaticMesh(SStaticMeshRP _staticMesh) noexcept { mStaticMeshResource = _staticMesh; }
	SStaticMeshRP GetStaticMesh() noexcept { return mStaticMeshResource; }

	void EmitRenderProxy(RStaticMeshProxy& _renderProxy) noexcept;

private:
	SStaticMeshRP mStaticMeshResource;
};

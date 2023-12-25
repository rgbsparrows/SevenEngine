#pragma once

#include "Render/RenderProxy/RenderProxy.h"
#include "Render/RenderProxy/World/WorldProxy.h"
#include "Classes/StaticMeshComponent.h"

struct SRenderScene
{
	void Init(SWorld* _world) noexcept;
	void Clear() noexcept;

	RRenderProxy<RWorld>* GetRenderProxy() noexcept { return mRenderWorld; }

	void UpdateRenderProxy() noexcept;

	void RegistComponent(ACRenderComponent* _component) noexcept;
	void UnregistComponent(ACRenderComponent* _component) noexcept;

	std::vector<ACStaticMesh*> mStaticMeshList;

	RRenderProxy<RWorld>* mRenderWorld;

	SWorld* mWorld = nullptr;
};

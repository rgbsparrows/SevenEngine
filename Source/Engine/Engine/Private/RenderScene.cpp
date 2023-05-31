#include "Engine/RenderScene.h"
#include "Core/Util/Algorithm.h"
#include "Render/RenderCommandList.h"

void SRenderScene::Init(SWorld* _world) noexcept
{
	mWorld = _world;
	mRenderWorld = GetRenderCommandList()->CreateRenderProxy<RWorld>();
}

void SRenderScene::Clear() noexcept
{
	CHECK(mStaticMeshList.empty());

	GetRenderCommandList()->AddExpiringRenderProxy(mRenderWorld);
}

void SRenderScene::UpdateRenderProxy() noexcept
{
	RWorld& renderWorld = mRenderWorld->Get_GameThread();

	renderWorld.mStaticMeshList.resize(mStaticMeshList.size());
	for (size_t i = 0; i < mStaticMeshList.size(); ++i)
	{
		if (mStaticMeshList[i]->IsRenderProxyDirty())
			mStaticMeshList[i]->EmitRenderProxy(renderWorld.mStaticMeshList[i]);
	}
}

void SRenderScene::RegistComponent(ACRenderComponent* _component) noexcept
{
	_component->MarkRenderProxyDirty();

	if (IsInstanceOf<ACStaticMesh>(_component))
		mStaticMeshList.push_back(static_cast<ACStaticMesh*>(_component));
}

void SRenderScene::UnregistComponent(ACRenderComponent* _component) noexcept
{
	if (IsInstanceOf<ACStaticMesh>(_component))
	{
		auto it = SwapEraseFirst(mStaticMeshList, static_cast<ACStaticMesh*>(_component));
		if (it != mStaticMeshList.end())
			(*it)->MarkRenderProxyDirty();
	}
}

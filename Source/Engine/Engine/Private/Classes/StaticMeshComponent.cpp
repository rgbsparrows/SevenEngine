#include "Render/RenderProxy/World/WorldProxy.h"
#include "Engine/Classes/StaticMeshComponent.h"

void ACStaticMesh::EmitRenderProxy(RStaticMeshProxy& _renderProxy) noexcept
{
	_renderProxy.mWorldMatrix = Math::CalcTransformMatrix(GetWorldTransform());
	_renderProxy.mMesh = mStaticMeshResource->GetRenderProxy();

	_renderProxy.mMeshColor.resize(mStaticMeshResource->GetSubMeshList().size());
	for(size_t i = 0; i < mStaticMeshResource->GetSubMeshList().size(); ++i)
		_renderProxy.mMeshColor[i] = Math::SFColor(1.0f, 1.0f, 1.0f, 1.0f);
}

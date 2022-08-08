#pragma once

#include "Core/Math/Type.h"
#include "Render/RenderPass/RenderGraph.h"
#include "Render/RenderProxy/RenderProxy.h"
#include "Render/RenderProxy/Resource/RenderResource.h"

#include <vector>

__interface IRDIBuffer;

struct RStaticMeshProxy
{
	Math::SFloat3 mPosition;
	Math::SFloat3 mRotation;
	Math::SFloat3 mScale;
	Math::SFloat4x4 mWorldMatrix;

	RRenderProxy<RMesh>* mMesh = nullptr;

	TODO("后续用材质替换")
	std::vector<Math::SFColor> mMeshColor;
};

struct RCamera
{
	Math::SFloat3 mPosition;
	Math::SFloat3 mRotation;
	float mFov;

	Math::SFloat4x4 mViewMatrix;
};

struct R3DWorldRenderData
{
	RCamera mCamera;
	std::vector<RStaticMeshProxy> mStaticMeshComponent;

	RRenderProxy<RTexture2D>* mRenderTarget;
	Math::SFloatRect mCanvasRect;
};
template<> struct RRenderInfoTraits<R3DWorldRenderData> : RExclusiveMode {};

using R3DWorldRenderGraph = TRenderGraph<R3DWorldRenderData>;

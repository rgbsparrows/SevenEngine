#pragma once

#include "Core/Math/Type.h"
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
	float mNearClip;
	float mFarClip;

	Math::SFloat4x4 mViewMatrix;
};

struct RWorld
{
	RCamera mCamera;
	std::vector<RStaticMeshProxy> mStaticMeshComponent;
};
template<> struct RRenderInfoTraits<RWorld> : RExclusiveMode {};

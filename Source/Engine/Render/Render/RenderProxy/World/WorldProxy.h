#pragma once

#include "Core/Math/Type.h"
#include "Render/RenderProxy/RenderProxy.h"
#include "Render/RenderProxy/Resource/RenderResource.h"

#include <vector>

__interface IRDIBuffer;

struct RStaticMeshProxy
{
	Math::SFloat4x4 mWorldMatrix;

	RRenderProxy<RMesh>* mMesh = nullptr;

	TODO("�����ò����滻")
	std::vector<Math::SFColor> mMeshColor;
};

struct RCamera
{
	Math::SFloat3 mPosition;
	Math::SFloat3 mRotation;
	float mFov;
	float mNearClip;
	float mFarClip;
};

struct RWorld
{
	std::vector<RStaticMeshProxy> mStaticMeshList;
};
template<> struct RRenderInfoTraits<RWorld> : RExclusiveMode {};

﻿#pragma once

#include "Core/Math/Type.h"
#include "Core/Math/Math.h"
#include "Render/RenderProxy/RenderProxy.h"
#include "Render/RenderProxy/Resource/RenderResource.h"

#include <vector>

__interface IRDIBuffer;

struct RStaticMeshProxy
{
	Math::SFloat4x4 mWorldMatrix;

	RRenderProxy<RMesh>* mMesh = nullptr;

	TODO("后续用材质替换")
	std::vector<Math::SFColor> mMeshColor;
};

struct RCamera
{
	Math::SFloat3 mPosition;
	Math::SFloat3 mRotation;
	float mFov = Math::PI / 4.f;
	float mNearClip = 1.f;
	float mFarClip = 10000.f;
};

struct RWorld
{
	std::vector<RStaticMeshProxy> mStaticMeshList;
};
template<> struct RRenderInfoTraits<RWorld> : RExclusiveMode {};

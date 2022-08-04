#pragma once

#include "Render/RenderProxy/RenderProxy.h"

#include <vector>

struct RStaticMesh
{

};

struct RCamera
{
};

struct RWorldProxy
{
	std::vector<RStaticMesh> mStaticMeshComponent;
};
template<> struct RRenderInfoTraits<RWorldProxy> : RExclusiveMode {};

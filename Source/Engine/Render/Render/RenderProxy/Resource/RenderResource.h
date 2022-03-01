#pragma once
#include "Core/Container/Blob.h"
#include "RDI/RDIFunctionHelper.h"
#include "RDI/Interface/RDIResource.h"
#include "Render/RenderProxy/RenderProxy.h"

struct RTexture2D
{
	IRDITexture2D* mTexture = nullptr;
};
template<> struct RRenderInfoTraits<RTexture2D> : RShareMode {};

struct RTexture2DData
{
	SRDITexture2DResourceDesc mDesc;

	std::vector<SBufferView> mSubresourceData;
	SBlob mResourceData;
};
template<> struct RRenderInfoTraits<RTexture2DData> : RExclusiveMode {};


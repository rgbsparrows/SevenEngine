#pragma once
#include "Render/RenderGraph/RenderGraph.h"
#include "Render/RenderProxy/World/WorldProxy.h"

struct RWorldRawRenderingData
{
	RWorld* mWorld = nullptr;
	RTexture2D* mRenderTarget = nullptr;
};

struct RWorldRenderingData
{
	RWorld* mWorld = nullptr;
	IRDITexture2D* mRenderTarget = nullptr;
};

using RWorldRenderPass = TRenderPass<RWorldRenderingData>;

class RWorldRenderGraph : public TRenderGraph<RWorldRawRenderingData, RWorldRenderingData>
{
	DECLARE_CLASSOBJECT_BODY(RWorldRenderGraph, TRenderGraph<RWorldRawRenderingData, RWorldRenderingData>)

protected:
	RRenderingData ProcessRawRenedrData(RRawRenderingData&& _renderingData) noexcept override
	{
		RRenderingData renderingData;
		renderingData.mWorld = _renderingData.mWorld;
		renderingData.mRenderTarget = _renderingData.mRenderTarget->mTexture;

		return renderingData;
	}
};

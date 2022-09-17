#pragma once
#include "Render/RenderGraph/RenderGraph.h"
#include "Render/RenderProxy/World/WorldProxy.h"

struct R3DWorldRawRenderingData
{
	R3DWorld* mWorld = nullptr;
	RTexture2D* mRenderTarget = nullptr;
};

struct R3DWorldRenderingData
{
	R3DWorld* mWorld = nullptr;
	IRDITexture2D* mRenderTarget = nullptr;
};

using R3DWorldRenderPass = TRenderPass<R3DWorldRenderingData>;

class R3DWorldRenderGraph : public TRenderGraph<R3DWorldRawRenderingData, R3DWorldRenderingData>
{
	DECLARE_CLASSOBJECT_BODY(R3DWorldRenderGraph, TRenderGraph<R3DWorldRawRenderingData, R3DWorldRenderingData>)

protected:
	RRenderingData ProcessRawRenedrData(RRawRenderingData&& _renderingData) noexcept override
	{
		RRenderingData renderingData;
		renderingData.mWorld = _renderingData.mWorld;
		renderingData.mRenderTarget = _renderingData.mRenderTarget->mTexture;

		return renderingData;
	}
};

#pragma once
#include "RenderGraph/WorldRenderGraph.h"

__interface IRDITexture2D;

class RWorldForwardRenderPass : public RWorldRenderPass
{
	DECLARE_CLASSOBJECT_BODY(RWorldForwardRenderPass, RWorldRenderPass)

public:
	void PreRender(RRenderingData& _renderSource, SSubRenderContent& _renderContent) noexcept override {}
	void Render(RRenderingData& _renderSource, IRDICommandList* _commandList) noexcept override;
	void PostRender(RRenderingData& _renderSource, SSubRenderContent& _renderContent) noexcept override {}

	void SetRenderTarget(IRDITexture2D* _renderTarget, IRDITexture2D* _depthTarget) noexcept
	{
		mRenderTarget = _renderTarget;
		mDepthTarget = _depthTarget;
	}

private:
	IRDITexture2D* mRenderTarget;
	IRDITexture2D* mDepthTarget;
};

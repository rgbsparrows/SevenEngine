#pragma once
#include "WorldRenderGraph.h"

class RWorldForwardRenderPass;

class RWorldForwardRenderGraph : public RWorldRenderGraph
{
public:
	void Init() noexcept;

protected:
	virtual void PrepareRenderPassResource(RRenderingData& _renderingData, SSubRenderContent& _renderContent) noexcept;
	virtual void PostRender(RRenderingData& _renderingData, SSubRenderContent& _renderContent) noexcept {}

private:
	RWorldForwardRenderPass* mForwardRenderPass;
};
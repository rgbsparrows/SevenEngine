#pragma once
#include "3DWorldRenderGraph.h"

class RWorldForwardRenderPass;

class RWorldForwardRenderGraph : public R3DWorldRenderGraph
{
public:
	void Init() noexcept;

protected:
	virtual void PrepareRenderPassResource(RRenderingData& _renderingData, SSubRenderContent& _renderContent) noexcept;
	virtual void PostRender(RRenderingData& _renderingData, SSubRenderContent& _renderContent) noexcept {}

private:
	R3DWorldForwardRenderPass* mForwardRenderPass;
};
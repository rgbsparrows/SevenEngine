#pragma once
#include "3DWorldRenderGraph.h"

class R3DWorldForwardRenderPass;

class R3DWorldForwardRenderGraph : public R3DWorldRenderGraph
{
public:
	void Init() noexcept;

protected:
	virtual void PrepareRenderPassResource(RRenderingData& _renderingData, SSubRenderContent& _renderContent) noexcept;
	virtual void PostRender(RRenderingData& _renderingData, SSubRenderContent& _renderContent) noexcept {}

private:
	R3DWorldForwardRenderPass* mForwardRenderPass;
};
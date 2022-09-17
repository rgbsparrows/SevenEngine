#include "RenderGraph/WorldForwardRenderGraph.h"
#include "RenderGraph/RenderPass/WorldForwardRenderPass.h"

void RWorldForwardRenderGraph::Init() noexcept
{
	mForwardRenderPass = new R3DWorldForwardRenderPass;

	std::vector<R3DWorldRenderPass*> renderPassList = { mForwardRenderPass };
	Setup(renderPassList);
}

void RWorldForwardRenderGraph::PrepareRenderPassResource(RRenderingData& _renderingData, SSubRenderContent& _renderContent) noexcept
{
	mForwardRenderPass->SetRenderTarget(_renderingData.mRenderTarget, nullptr);
}

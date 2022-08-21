#include "3DWorldForwardRenderGraph.h"
#include "RenderPass/3DWorldForwardRenderPass.h"

void R3DWorldForwardRenderGraph::Init() noexcept
{
	mForwardRenderPass = new R3DWorldForwardRenderPass;

	std::vector<R3DWorldRenderPass*> renderPassList = { mForwardRenderPass };
	Setup(renderPassList);
}

void R3DWorldForwardRenderGraph::PrepareRenderPassResource(RRenderingData& _renderingData, SSubRenderContent& _renderContent) noexcept
{
	mForwardRenderPass->SetRenderTarget(_renderingData.mRenderTarget, nullptr);
}

#include "RenderGraph/RenderPass/3DWorldForwardRenderPass.h"

void R3DWorldForwardRenderPass::Render(RRenderingData& _renderSource, IRDICommandList* _commandList) noexcept
{
	_commandList->TranstionResourceState(mRenderTarget, ERDIResourceState::Common, ERDIResourceState::RenderTarget);
	_commandList->ClearRenderTargetView(mRenderTarget->GetRTV(0));
	_commandList->TranstionResourceState(mRenderTarget, ERDIResourceState::RenderTarget, ERDIResourceState::Common);
}

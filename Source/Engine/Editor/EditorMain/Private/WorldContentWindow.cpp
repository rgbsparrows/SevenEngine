#include "UI/Imgui/imgui.h"
#include "WorldContentWindow.h"
#include "Render/RenderCommandList.h"
#include "RenderGraph/3DWorldForwardRenderGraph.h"

#include <format>

void SUIWorldContentWindow::OnGui() noexcept
{
	if (mIsWindowOpen == false)
		return;

	static RRenderProxy<R3DWorld>* renderProxy = new RRenderProxy<R3DWorld>;
	if (mRenderGraph == nullptr)
	{
		mRenderGraph = new R3DWorldForwardRenderGraph;
		mRenderGraph->Init();
		GetRenderModule()->GetRenderCommandList()->ConstructRenderGraph(mRenderGraph);

		mRT = new RRenderProxy<RTexture2D>;
		RTexture2DData texData;
		texData.mDesc.mResourceUsage = ERDIResourceUsage::RenderTarget | ERDIResourceUsage::ShaderResource;
		texData.mDesc.mPixelFormat = ERDIPixelFormat::R8G8B8A8_UNORM;
		texData.mDesc.mSizeX = 1024;
		texData.mDesc.mSizeY = 1024;
		texData.mDesc.mClearColor = Math::SFColor(0.5f, 0, 0, 1);
		GetRenderModule()->GetRenderCommandList()->RefrashStaticTexture2D_I(mRT, std::move(texData));

		mImTex = new RRenderProxy<RImguiTexture2D>;
		GetRenderModule()->GetRenderCommandList()->RefrashImTexture2D_I(mRT, mImTex);
	}

	ImGui::Begin(std::format("WorldContentWindow [{0}]", static_cast<const void*>(this)).c_str(), &mIsWindowOpen);

	GetRenderModule()->GetRenderCommandList()->RenderWorld(renderProxy, mRT, mRenderGraph);

	ImGui::Image(mImTex, ImGui::GetContentRegionAvail());

	ImGui::End();
}

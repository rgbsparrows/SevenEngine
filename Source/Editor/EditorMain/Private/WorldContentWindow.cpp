#include "Engine/World.h"
#include "Engine/Engine.h"
#include "WorldContentWindow.h"
#include "Renderer/BaseRenderer.h"
#include "Resource/ResourceModule.h"
#include "Render/RenderCommandList.h"
#include "Resource/ResourceType/StaticMeshResource.h"

#include <format>
#include "imgui.h"

SUIWorldContentWindow::SUIWorldContentWindow() noexcept
{
	mRT = GetRenderCommandList()->CreateRenderProxy<RTexture2D>();
	mImTex = GetRenderCommandList()->CreateRenderProxy<RImguiTexture2D>();

	mRenderGraph = GetRenderCommandList()->CreateRenderGraph<RBaseRenderer>();
}

void SUIWorldContentWindow::OnGui() noexcept
{
	if (mIsWindowOpen == false)
		return;

	ImGui::Begin(std::format("WorldContentWindow [{0}]", static_cast<const void*>(this)).c_str(), &mIsWindowOpen);
	
	HandleWindowResize();

	RenderWorld();

	ImGui::Image(mImTex, ImGui::GetContentRegionAvail());

	ImGui::End();
}

void SUIWorldContentWindow::HandleWindowResize()
{
	ImVec2 Size = ImGui::GetWindowSize();
	if (Size.x != mLastWindowSize[0] || Size.y != mLastWindowSize[1])
	{
		mLastWindowSize[0] = Size.x;
		mLastWindowSize[1] = Size.y;
		OnResize();
	}
}

void SUIWorldContentWindow::OnResize()
{
	ImVec2 WindowSize = ImGui::GetWindowSize();

	if (WindowSize.x > 0 && WindowSize.y > 0)
	{
		RTexture2DData texData;
		texData.mDesc.mResourceUsage = ERDIResourceUsage::RenderTarget | ERDIResourceUsage::ShaderResource;
		texData.mDesc.mPixelFormat = ERDIPixelFormat::R8G8B8A8_UNORM;
		texData.mDesc.mSizeX = static_cast<uint32_t>(WindowSize.x);
		texData.mDesc.mSizeY = static_cast<uint32_t>(WindowSize.y);
		texData.mDesc.mClearColor = Math::SFColor(0.5f, 0, 0, 1);
		GetRenderCommandList()->RefrashStaticTexture2D_I(mRT, std::move(texData));
		GetRenderCommandList()->RefrashImTexture2D_I(mRT, mImTex);
	}
}

void SUIWorldContentWindow::RenderWorld()
{
	if (SWorld* World = SEngine::Get().GetMainWorld())
	{
		GetRenderCommandList()->RenderWorld_D(World->GetRenderProxy(), RCamera(), mRT, mRenderGraph);
	}
}

#pragma once

#include "UI/WindowInterface.h"
#include "Render/RenderProxy/WindowInfo.h"

class RWorldForwardRenderGraph;

class SUIWorldContentWindow : public IUIWindowInterface
{
public:
	bool IsWindowOpen() noexcept { return mIsWindowOpen; }

	void OnGui() noexcept;
	void OnClose() noexcept {}
	void Release() noexcept {}

private:
	bool mIsWindowOpen = true;

	RRenderProxy<RTexture2D>* mRT = nullptr;
	RRenderProxy<RImguiTexture2D>* mImTex = nullptr;
	RWorldForwardRenderGraph* mRenderGraph = nullptr;
};

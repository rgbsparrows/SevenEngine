#pragma once

#include "UI/WindowInterface.h"
#include "GameFramework/World.h"
#include "Render/RenderProxy/WindowInfo.h"

class R3DWorldForwardRenderGraph;

class SUIWorldContentWindow : public IUIWindowInterface
{
public:
	bool IsWindowOpen() noexcept { return mIsWindowOpen; }

	void OnGui() noexcept;
	void OnClose() noexcept {}
	void Release() noexcept {}

private:
	bool mIsWindowOpen = true;

	GameFramework::UWorldBase* mWorld = nullptr;
	RRenderProxy<RTexture2D>* mRT = nullptr;
	RRenderProxy<RImguiTexture2D>* mImTex = nullptr;
	R3DWorldForwardRenderGraph* mRenderGraph = nullptr;
};

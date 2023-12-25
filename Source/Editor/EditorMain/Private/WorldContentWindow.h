#pragma once

#include "UI/WindowInterface.h"
#include "Render/RenderProxy/WindowInfo.h"

__interface RRenderGraphBase;

class SUIWorldContentWindow : public IUIWindowInterface
{
public:
	SUIWorldContentWindow() noexcept;

	bool IsWindowOpen() noexcept { return mIsWindowOpen; }

	void OnGui() noexcept;
	void Release() noexcept {}

protected:
	void HandleWindowResize();
	void OnResize();

	void RenderWorld();

private:
	bool mIsWindowOpen = true;

	Math::SFloat2 mLastWindowSize;

	RRenderProxy<RTexture2D>* mRT = nullptr;
	RRenderProxy<RImguiTexture2D>* mImTex = nullptr;
	RRenderGraphBase* mRenderGraph = nullptr;
};

#pragma once
#include "Core/Math/Type.h"
#include "Core/Misc/windowsEx.h"
#include "Render/RenderProxy/WindowInfo.h"

#include "imgui.h"

class SUIInternalWindow
{
public:
	void Init(ImGuiViewport* _viewport);
	void Release();

	void ShowWindow() noexcept;
	void SetWindowPos(Math::SFloat2 _pos) noexcept;
	Math::SFloat2 GetWindowPos() noexcept;
	void SetWindowSize(Math::SFloat2 _size) noexcept;
	Math::SFloat2 GetWindowSize() noexcept;
	void SetWindowFocus() noexcept;
	bool GetWindowFocus() noexcept;
	bool GetWindowMinimized() noexcept;
	void SetWindowTitle(std::string_view _title) noexcept;
	void SetWindowAlpha(float _alpha) noexcept;

	void FlushImguiDrawData() noexcept;

public:
	HWND GetHwnd() noexcept { return mHwnd; }

private:
	HWND mHwnd = nullptr;
	DWORD mWndStyle = 0;
	DWORD mWndExStyle = 0;
	ImGuiViewport* mImguiViewport = nullptr;

	RRenderProxy<RSwapChain>* mSwapChain = nullptr;
	RRenderProxy<RImguiDrawData>* mDrawData = nullptr;

	RSwapChainData mSwapChainData;

	bool mIsSwapChainDiry = false;
};

#pragma once
#include "UIWindow.h"
#include "UI/UIModule.h"
#include "Core/Misc/windowsEx.h"

#include "Core/Misc/PreWindowsApi.h"
#include <Xinput.h>
#include "Core/Misc/PostWindowsApi.h"

#include <string>
#include <map>

class SUIModuleImpl : public IUIModuleInterface
{
public:
	bool Init() noexcept override;
	void Clear() noexcept override;

	TODO("Imgui NewFrame调用，各个窗口的OnGui，窗口的渲染调用")
	void OnGUI() noexcept override;

	void AddWindow(const std::wstring& _windowTag, UWindowInterface* _window) noexcept override;
	UWindowInterface* GetWindowByTag(const std::wstring& _windowTag) noexcept override;

private:
	void InitImgui() noexcept;
	void InitImguiConfig() noexcept;
	void InitImguiCallBack() noexcept;

	void ClearImgui() noexcept;

	void ProcessWndMessage() noexcept;
	void ImguiNewFrame() noexcept;
	void ImguiEndFrame() noexcept;

	void RegistWindowClass() noexcept;
	void UnregistWindowClass() noexcept;

	static LRESULT CALLBACK WndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

private:
	using SXInputGetCapabilitiesFuncType = DWORD(WINAPI)(DWORD, DWORD, XINPUT_CAPABILITIES*);
	using SXInputGetStateFuncType = DWORD(WINAPI)(DWORD, XINPUT_STATE*);

private:
	SUIInternalWindow* mMainWindow = nullptr;
	std::vector<SUIInternalWindow*> mAdditionWindow;

	std::map<std::wstring, UWindowInterface*> mUIWindows;
	std::vector<UWindowInterface*> mAnonymousUIWindows;

	RRenderProxy<RTexture2D>* mFontTexture;
	RRenderProxy<RImguiTexture2D>* mImFontTexture;
	RDirtyFlag mImFontTextureDirtyFlag;

	HICON mIcon = nullptr;
	HMODULE mXInputDLL = nullptr;
	SXInputGetCapabilitiesFuncType* mXInputGetCapabilitiesFunc = nullptr;
	SXInputGetStateFuncType* mXInputGetStateFunc = nullptr;
};

SUIModuleImpl* GetUIModuleImpl() noexcept;

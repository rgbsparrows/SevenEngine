#pragma once
#include "UIWindow.h"
#include "UI/UIModule.h"
#include "Core/Misc/windowsEx.h"

#include "Core/Misc/PreWindowsApi.h"
#include <Xinput.h>
#include <shellscalingapi.h>
#include "Core/Misc/PostWindowsApi.h"

#include <string>
#include <map>

class SUIModuleImpl : public IUIModuleInterface
{
public:
	bool Init() noexcept override;
	void Clear() noexcept override;

	void OnGUI() noexcept override;

	void AddWindow(const std::wstring& _windowTag, IUIWindowInterface* _window) noexcept override;
	IUIWindowInterface* GetWindowByTag(const std::wstring& _windowTag) noexcept override;

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
	static BOOL CALLBACK UpdateMonitors_EnumFunc(HMONITOR monitor, HDC, LPRECT, LPARAM);

private:
	using SXInputGetCapabilitiesFuncType = DWORD(WINAPI)(DWORD, DWORD, XINPUT_CAPABILITIES*);
	using SXInputGetStateFuncType = DWORD(WINAPI)(DWORD, XINPUT_STATE*);
	using SGetDpiForMonitorFuncType = HRESULT(WINAPI)(HMONITOR, MONITOR_DPI_TYPE, UINT*, UINT*);

private:
	SUIInternalWindow* mMainWindow = nullptr;
	bool mIsMainWindowOpen = true;
	std::vector<SUIInternalWindow*> mAdditionWindow;

	std::map<std::wstring, IUIWindowInterface*> mUIWindows;
	std::vector<IUIWindowInterface*> mAnonymousUIWindows;

	RRenderProxy<RTexture2D>* mFontTexture = nullptr;
	RRenderProxy<RImguiTexture2D>* mImFontTexture = nullptr;

	HICON mIcon = nullptr;
	HMODULE mXInputDLL = nullptr;
	SXInputGetCapabilitiesFuncType* mXInputGetCapabilitiesFunc = nullptr;
	SXInputGetStateFuncType* mXInputGetStateFunc = nullptr;
};

SUIModuleImpl* GetUIModuleImpl() noexcept;

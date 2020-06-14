#pragma once

#include "UIBackPlane.h"
#include "WindowsTypes.h"
#include "WindowsConstants.h"
#include "UIInterface/UIWinMessageHandlerInterface.h"

class SUIWindow : public SUIBackPlane, public IUIWinMessageHandler
{
	DECLARE_CLASSOBJECT_BODY(SUIWindow, SUIBackPlane)
public:
	bool Create(
		std::wstring_view _windowName,
		EWinWindowStyle _windowStyle,
		Math::SIntRect _rect
	) noexcept;
	
	HWND GetRawHandle() const noexcept { return mHwnd; }

	bool IsAvailable() const noexcept { return mHwnd != nullptr; }
	void SetTitle(std::wstring_view _title) noexcept;

	bool ProcessMessage(EWinMessage _message, uintptr_t _wparam, intptr_t _lparam) noexcept override;
	void OnRelease() noexcept override;

private:
	HWND mHwnd = nullptr;
};

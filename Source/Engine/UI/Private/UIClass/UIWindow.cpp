#include "UIModuleImpl.h"
#include "UI/UIClass/UIWindow.h"
#include "WindowsPlatform/ErrorCode.h"
#include "WindowsPlatform/WindowsPlatformApi.h"

bool SUIWindow::Create(std::wstring_view _windowName, EWinWindowStyle _windowStyle, Math::SIntRect _rect) noexcept
{
	SErrorCode errorCode;
	mHwnd = SWindowsPlatformApi::CreateWindow(
		GetUIModuleImpl()->GetDefaultWindowClassName(),
		_windowName,
		_windowStyle,
		_rect.mLeft,
		_rect.mOn,
		_rect.GetWidth(),
		_rect.GetHeight(),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		&errorCode);

	if (errorCode.IsSuccess())
		return false;

	GetUIModuleImpl()->RegistWindow(mHwnd, this);

	OnCreated();

	return true;
}

void SUIWindow::SetTitle(std::wstring_view _title) noexcept
{
	SWindowsPlatformApi::SetWindowTitle(mHwnd, _title);
}

bool SUIWindow::ProcessMessage(EWinMessage _message, uintptr_t _wparam, intptr_t _lparam) noexcept
{
	switch (_message)
	{
	case EWinMessage::WM_CLOSE:
		SWindowsPlatformApi::DestroyWindow(mHwnd, nullptr);
		return true;
	case EWinMessage::WM_NCDESTROY:
		Release();
		return true;
	default:
		return false;
	}
}

void SUIWindow::OnRelease() noexcept
{
	GetUIModuleImpl()->UnregistWindow(mHwnd);
	mHwnd = nullptr;

	BaseClass::OnRelease();
}

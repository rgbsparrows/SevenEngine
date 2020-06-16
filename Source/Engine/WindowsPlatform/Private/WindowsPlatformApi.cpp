#include "Core/Localize.h"
#include "WindowsPlatformApiImplWarper.h"
#include "WindowsPlatform/WindowsPlatformApi.h"

std::wstring SWindowsPlatformApi::ConvertStringToWstring(uint32_t _codePage, std::string_view _srcString) noexcept
{
	return WindowsPlatformApiWarp_Impl::ConvertStringToWstring_WindowsImpl(_codePage, _srcString);
}

std::string SWindowsPlatformApi::ConvertWstringToString(uint32_t _codePage, std::wstring_view _srcString) noexcept
{
	return WindowsPlatformApiWarp_Impl::ConvertWstringToString_WindowsImpl(_codePage, _srcString);
}

EWinMessboxResult SWindowsPlatformApi::MessageBox(HWND _hwnd, std::wstring_view _text, std::wstring_view _cation, EWinMessageBoxFlag _type) noexcept
{
	uint32_t result = WindowsPlatformApiWarp_Impl::MessageBox_WindowsImpl(reinterpret_cast<void*>(_hwnd), _text, _cation, static_cast<uint32_t>(_type));
	return static_cast<EWinMessboxResult>(result);
}

std::wstring SWindowsPlatformApi::GetCommandLine() noexcept
{
	return WindowsPlatformApiWarp_Impl::GetCommandLine_WindowsImpl();
}

std::filesystem::path SWindowsPlatformApi::GetModulePath(HMODULE _module, SErrorCode* _errorCode) noexcept
{
	return WindowsPlatformApiWarp_Impl::GetModulePath_WindowsImpl(reinterpret_cast<void*>(_module), _errorCode ? reinterpret_cast<uint32_t*>(&_errorCode->mErrorCode) : nullptr);
}

HMODULE SWindowsPlatformApi::LoadLibary(const std::filesystem::path& _modulePath, SErrorCode* _errorCode) noexcept
{
	void* dll = WindowsPlatformApiWarp_Impl::LoadLibary_WindowsImpl(_modulePath, _errorCode ? reinterpret_cast<uint32_t*>(&_errorCode->mErrorCode) : nullptr);
	return reinterpret_cast<HMODULE>(dll);
}

void SWindowsPlatformApi::FreeLibrary(HMODULE _module, SErrorCode* _errorCode) noexcept
{
	WindowsPlatformApiWarp_Impl::FreeLibrary_WindowsImpl(_module, _errorCode ? reinterpret_cast<uint32_t*>(&_errorCode->mErrorCode) : nullptr);
}

void* SWindowsPlatformApi::LoadRawProcAddress(HMODULE _module, std::wstring_view _funcSignature, SErrorCode* _errorCode) noexcept
{
	return WindowsPlatformApiWarp_Impl::LoadProcAddress_WindowsImpl(_module, ConvertWstringToString(Locale::ECodePage::ACP, _funcSignature), _errorCode ? reinterpret_cast<uint32_t*>(&_errorCode->mErrorCode) : nullptr);
}

void SWindowsPlatformApi::OutputDebugString(std::wstring_view _message) noexcept
{
	WindowsPlatformApiWarp_Impl::OutputDebugString_WindowsImpl(_message);
}

bool SWindowsPlatformApi::RegisterClass(EWinClassStyle _style, SWndProcFuncType* _wndProc, HINSTANCE _instance, HICON _icon, HCURSOR _cursor, HBRUSH _background, std::wstring_view _menuName, std::wstring_view _classsName, HICON _samllIcon, SErrorCode* _errorCode) noexcept
{
	return WindowsPlatformApiWarp_Impl::RegisterClass_WindowsImpl(EnumToInt(_style), _wndProc, _instance, _icon, _cursor, _background, _menuName, _classsName, _samllIcon, _errorCode ? reinterpret_cast<uint32_t*>(&_errorCode->mErrorCode) : nullptr);
}

bool SWindowsPlatformApi::GetClassInfo(HINSTANCE* _instance, std::wstring_view _className, EWinClassStyle* _classStyle, SWndProcFuncType** _wndProc, HICON* _icon, HCURSOR* _cursor, HBRUSH* _background, HICON* _smallIcon, SErrorCode* _errorCode) noexcept
{
	return WindowsPlatformApiWarp_Impl::GetClassInfo_WindowsImpl(_instance, _className, reinterpret_cast<uint32_t*>(&_classStyle), reinterpret_cast<void**>(_wndProc), reinterpret_cast<void**>(_icon), reinterpret_cast<void**>(_cursor), reinterpret_cast<void**>(_background), reinterpret_cast<void**>(_smallIcon), _errorCode ? reinterpret_cast<uint32_t*>(&_errorCode->mErrorCode) : nullptr);
}

bool SWindowsPlatformApi::UnregisterClass(std::wstring_view _className, HINSTANCE _instance, SErrorCode* _errorCode) noexcept
{
	return WindowsPlatformApiWarp_Impl::UnregisterClass_WindowsImpl(_className, _instance, _errorCode ? reinterpret_cast<uint32_t*>(&_errorCode->mErrorCode) : nullptr);
}

HWND SWindowsPlatformApi::CreateWindow(std::wstring_view _className, std::wstring_view _windowName, EWinWindowStyle _style, int32_t _x, int32_t _y, int32_t _width, int32_t _height, HWND _wndParent, HMENU _menu, HINSTANCE _instance, void* _param, SErrorCode* _errorCode) noexcept
{
	void* wnd = WindowsPlatformApiWarp_Impl::CreateWindow_WindowsImpl(_className, _windowName, EnumToInt(_style), _x, _y, _width, _height, _wndParent, _menu, _instance, _param, _errorCode ? reinterpret_cast<uint32_t*>(&_errorCode->mErrorCode) : nullptr);
	return reinterpret_cast<HWND>(wnd);
}

HWND SWindowsPlatformApi::FindWindow(HWND _wndParent, HWND _wndChildAfter, std::wstring_view _className, std::wstring_view _windowName, SErrorCode* _errorCode) noexcept
{
	void* wnd = WindowsPlatformApiWarp_Impl::FindWindow_WindowsImpl(_wndParent, _wndChildAfter, _className, _windowName, _errorCode ? reinterpret_cast<uint32_t*>(&_errorCode->mErrorCode) : nullptr);
	return reinterpret_cast<HWND>(wnd);
}

bool SWindowsPlatformApi::DestroyWindow(HWND _wnd, SErrorCode* _errorCode) noexcept
{
	return WindowsPlatformApiWarp_Impl::DestroyWindow_WindowsImpl(_wnd, _errorCode ? reinterpret_cast<uint32_t*>(&_errorCode->mErrorCode) : nullptr);
}

intptr_t SWindowsPlatformApi::DefWindowProc(HWND _wnd, EWinMessage _message, uintptr_t _wparam, intptr_t _lparam) noexcept
{
	return WindowsPlatformApiWarp_Impl::DefWindowProc_WindowsImpl(_wnd, static_cast<uint32_t>(_message), _wparam, _lparam);
}

void SWindowsPlatformApi::ProcessWinMessage() noexcept
{
	WindowsPlatformApiWarp_Impl::ProcessWinMessage_WindowsImpl();
}

bool SWindowsPlatformApi::IsWinMessageQueueClose() noexcept
{
	return WindowsPlatformApiWarp_Impl::IsWinMessageQueueClose_WindowsImpl();
}

bool SWindowsPlatformApi::SetWindowTitle(HWND _wnd, std::wstring_view _title) noexcept
{
	return WindowsPlatformApiWarp_Impl::SetWindowTitle_WindowsImpl(_wnd, _title);
}

#pragma once

#include "WindowsConstants.h"
#include "WindowsTypes.h"
#include "ErrorCode.h"

#include <string>
#include <string_view>
#include <filesystem>

struct SWindowsPlatformApi
{
	static std::wstring ConvertStringToWstring(uint32_t _codePage, std::string_view _srcString) noexcept;
	static std::string ConvertWstringToString(uint32_t _codePage, std::wstring_view _srcString) noexcept;

	static EMessboxResult MessageBox(HWND _hwnd, std::wstring_view _text, std::wstring_view _cation, EMessageBoxFlag _type) noexcept;

	static std::wstring GetCommandLine() noexcept;

	static std::filesystem::path GetModulePath(HMODULE _module, SErrorCode* _errorCode = nullptr) noexcept;

	static HMODULE LoadLibary(const std::filesystem::path& _modulePath, SErrorCode* _errorCode = nullptr) noexcept;
	static void FreeLibrary(HMODULE _module, SErrorCode* _errorCode = nullptr) noexcept;

	template<typename _functionType>
	static _functionType* LoadProcAddress(HMODULE _module, std::wstring_view _funcSignature, SErrorCode* _errorCode = nullptr) noexcept
	{
		using FunctionType = _functionType;
		return reinterpret_cast<FunctionType*>(LoadRawProcAddress(_module, _funcSignature, _errorCode));
	}

	static void* LoadRawProcAddress(HMODULE _module, std::wstring_view _funcSignature, SErrorCode* _errorCode = nullptr) noexcept;

	static void OutputDebugString(std::wstring_view _message) noexcept;

	static bool RegisterClass(EClassStyle _style, SWndProcFuncType* _wndProc, HINSTANCE _instance, HICON _icon, HCURSOR _cursor, HBRUSH _background, std::wstring_view _classsName, HICON _samllIcon, SErrorCode* _errorCode) noexcept;
	static bool GetClassInfo(HINSTANCE* _instance, std::wstring_view _className, _Out_ EClassStyle* _classStyle, _Out_ SWndProcFuncType** _wndProc, _Out_ HICON* _icon, _Out_ HCURSOR* _cursor, _Out_ HBRUSH* _background, _Out_ HICON* _smallIcon, SErrorCode* _errorCode) noexcept;
	static bool UnregisterClass(std::wstring_view _className, HINSTANCE _instance, SErrorCode* _errorCode) noexcept;

	static HWND CreateWindow(std::wstring_view _className, std::wstring_view _windowName, EWindowStyle _style, int32_t _x, int32_t _y, int32_t _width, int32_t _height, HWND _wndParent, HMENU _menu, HINSTANCE _instance, void* _param, SErrorCode* _errorCode) noexcept;
	static HWND FindWindow(HWND _wndParent, HWND _wndChildAfter, std::wstring_view _className, std::wstring_view _windowName, SErrorCode* _errorCode) noexcept;
	static bool DestroyWindow(HWND _wnd, SErrorCode* _errorCode) noexcept;
};

#pragma once

#include "ErrorCode.h"
#include "WindowsTypes.h"
#include "WindowsConstants.h"

#include <string>
#include <filesystem>
#include <string_view>

using SWndProcFuncType = intptr_t _stdcall(HWND, EWinMessage, uintptr_t, intptr_t);

struct SWindowsPlatformApi
{
	static std::wstring ConvertStringToWstring(uint32_t _codePage, std::string_view _srcString) noexcept;
	static std::string ConvertWstringToString(uint32_t _codePage, std::wstring_view _srcString) noexcept;

	static EWinMessboxResult MessageBox(HWND _hwnd, std::wstring_view _text, std::wstring_view _cation, EWinMessageBoxFlag _type) noexcept;

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

	static bool RegisterClass(EWinClassStyle _style, SWndProcFuncType* _wndProc, HINSTANCE _instance, HICON _icon, HCURSOR _cursor, HBRUSH _background, std::wstring_view _menuName, std::wstring_view _classsName, HICON _samllIcon, SErrorCode* _errorCode) noexcept;
	static bool GetClassInfo(HINSTANCE* _instance, std::wstring_view _className, EWinClassStyle* _classStyle, SWndProcFuncType** _wndProc, HICON* _icon, HCURSOR* _cursor, HBRUSH* _background, HICON* _smallIcon, SErrorCode* _errorCode) noexcept;
	static bool UnregisterClass(std::wstring_view _className, HINSTANCE _instance, SErrorCode* _errorCode) noexcept;

	static HWND CreateWindow(std::wstring_view _className, std::wstring_view _windowName, EWinWindowStyle _style, int32_t _x, int32_t _y, int32_t _width, int32_t _height, HWND _wndParent, HMENU _menu, HINSTANCE _instance, void* _param, SErrorCode* _errorCode) noexcept;
	static HWND FindWindow(HWND _wndParent, HWND _wndChildAfter, std::wstring_view _className, std::wstring_view _windowName, SErrorCode* _errorCode) noexcept;
	static bool DestroyWindow(HWND _wnd, SErrorCode* _errorCode) noexcept;

	static intptr_t DefWindowProc(HWND _wnd, EWinMessage _message, uintptr_t _wparam, intptr_t _lparam) noexcept;

	static void ProcessWinMessage() noexcept;
	static bool IsWinMessageQueueClose() noexcept;

	static bool SetWindowTitle(HWND _wnd, std::wstring_view _title) noexcept;
};

#pragma once

#include <string>
#include <string_view>
#include <filesystem>

namespace WindowsPlatformApiWarp_Impl
{
	std::wstring ConvertStringToWstring_WindowsImpl(uint32_t _codePage, std::string_view _srcString) noexcept;
	std::string ConvertWstringToString_WindowsImpl(uint32_t _codePage, std::wstring_view _srcString) noexcept;
	uint32_t MessageBox_WindowsImpl(void* _hwnd, std::wstring_view _text, std::wstring_view _caption, uint32_t _type) noexcept;
	std::wstring GetCommandLine_WindowsImpl() noexcept;
	std::filesystem::path GetModulePath_WindowsImpl(void* _module, uint32_t* _errorCode) noexcept;
	void* LoadLibary_WindowsImpl(const std::filesystem::path& _modulePath, uint32_t* _errorCode) noexcept;
	void FreeLibrary_WindowsImpl(void* _module, uint32_t* _errorCode) noexcept;
	void* LoadProcAddress_WindowsImpl(void* _module, const std::string& _procName, uint32_t* _errorCode) noexcept;
	std::wstring FormatMessage_WindowsImpl(uint32_t _errorCode) noexcept;
	void OutputDebugString_WindowsImpl(std::wstring_view _message) noexcept;
	bool RegisterClass_WindowsImpl(uint32_t _style, void* _wndProc, void* _instance, void* _icon, void* _cursor, void* _background, std::wstring_view _className, void* _smallIcon, uint32_t* _errorCode) noexcept;
	bool GetClassInfo_WindowsImpl(void* _instance, std::wstring_view _className, uint32_t* _style, void** _wndProc, void** _icon, void** _cursor, void** _background, void** _smallIcon, uint32_t* _errorCode) noexcept;
	bool UnregisterClass_WindowsImpl(std::wstring_view _className, void* _instance, uint32_t* _errorCode) noexcept;
	void* CreateWindow_WindowsImpl(std::wstring_view _className, std::wstring_view _windowName, uint64_t _style, int32_t _x, int32_t _y, int32_t _width, int32_t _height, void* _wndParent, void* _menu, void* _instance, void* _param, uint32_t* _errorCode) noexcept;
	void* FindWindow_WindowsImpl(void* _wndParent, void* _wndChildAfter, std::wstring_view _className, std::wstring_view _windowName, uint32_t* _errorCode) noexcept;
	bool DestroyWindow_WindowsImpl(void* _wnd, uint32_t* _errorCode) noexcept;
}

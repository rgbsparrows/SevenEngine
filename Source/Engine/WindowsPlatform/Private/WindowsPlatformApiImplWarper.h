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
}

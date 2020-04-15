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
};

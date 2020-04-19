#pragma once

#include "WindowsPlatformApi.h"
#include "Macros/ConditionDeprecated.h"

DEPRECATED_WHEN_FMT_LIBRARY_AVAILABLE("")
inline void CombinString() noexcept
{
}

template <typename T>
DEPRECATED_WHEN_FMT_LIBRARY_AVAILABLE("")
std::wstring CombinString(const T& t) noexcept
{
	return std::wstring() + t;
}

template <typename First, typename... Rest>
DEPRECATED_WHEN_FMT_LIBRARY_AVAILABLE("")
std::wstring CombinString(const First& first, const Rest&... rest) noexcept 
{
	return std::wstring() + first + CombinString(rest...);
}

template<typename... _argts>
DEPRECATED_WHEN_SE_LOG_AVAILABLE("")
void LogInfo(_argts&&... _args) noexcept
{
	std::wstring message = CombinString(L"INFO: ", _args...);
	SWindowsPlatformApi::OutputDebugString(message);
}

template<typename... _argts>
DEPRECATED_WHEN_SE_LOG_AVAILABLE("")
void LogWarn(_argts&&... _args) noexcept
{
	std::wstring message = CombinString(L"WARN: ", _args...);
	SWindowsPlatformApi::OutputDebugString(message);
}

template<typename... _argts>
DEPRECATED_WHEN_SE_LOG_AVAILABLE("")
void LogErr(_argts&&... _args) noexcept
{
	std::wstring message = CombinString(L"ERROR: ", _args...);
	SWindowsPlatformApi::OutputDebugString(message);
	SWindowsPlatformApi::MessageBox(nullptr, message, L"LOG ERR", EMessageBoxFlag::MB_OK);
}

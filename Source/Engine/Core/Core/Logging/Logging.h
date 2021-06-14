#pragma once

#include "Core/ProgramConfiguation/ConditionDeprecated.h"
#include "Core/Misc/windowsEx.h"

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
	OutputDebugString(message);
}

template<typename... _argts>
DEPRECATED_WHEN_SE_LOG_AVAILABLE("")
void LogWarn(_argts&&... _args) noexcept
{
	std::wstring message = CombinString(L"WARN: ", _args...);
	OutputDebugString(message);
}

template<typename... _argts>
DEPRECATED_WHEN_SE_LOG_AVAILABLE("")
void LogErr(_argts&&... _args) noexcept
{
	std::wstring message = CombinString(L"ERROR: ", _args...);
	OutputDebugString(message);
	MessageBoxW(nullptr, message, L"LOG ERR", 0);
}

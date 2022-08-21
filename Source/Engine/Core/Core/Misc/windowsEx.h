#pragma once

#include "Core/Misc/PreWindowsApi.h"
#include <windows.h>
#include <synchapi.h>
#include "Core/Misc/PostWindowsApi.h"

#include "Core/Util/UtilMacros.h"

#include <string>
#include <filesystem>

extern "C" _ACRTIMP void __cdecl _wassert(
	_In_z_ wchar_t const* _Message,
	_In_z_ wchar_t const* _File,
	_In_   unsigned       _Line
);

template<typename _functionType>
auto GetProcAddress(HMODULE _module, const char* _funcSignature) noexcept
{
	using FunctionType = _functionType;
	return reinterpret_cast<FunctionType*>(::GetProcAddress(_module, _funcSignature));
}

std::wstring GetHResultMessage(HRESULT _hresult) noexcept;

void YieldForSingleObject(HANDLE _handle) noexcept;

#pragma once

#include "Core/PreWindowsApi.h"
#include <windows.h>
#include <synchapi.h>
#include "Core/PostWindowsApi.h"

#include "Macros/UtilMacros.h"

#include <string>
#include <filesystem>

extern "C" _ACRTIMP void __cdecl _wassert(
	_In_z_ wchar_t const* _Message,
	_In_z_ wchar_t const* _File,
	_In_   unsigned       _Line
);

template<typename _functionType>
auto GetProcAddress(HMODULE _module, std::string_view _funcSignature) noexcept
{
	using FunctionType = _functionType;
	return reinterpret_cast<FunctionType*>(::GetRawProcAddress(_module, _funcSignature));
}

void* GetRawProcAddress(HMODULE _module, std::string_view _funcSignature) noexcept;
void OutputDebugString(std::wstring_view _message) noexcept;

void ProcessWinMessage() noexcept;
bool IsWinMessageQueueClose() noexcept;

void YieldForSingleObject(HANDLE _handle) noexcept;

struct SCriticalSection
{
	SCriticalSection() noexcept
	{
		bool res = InitializeCriticalSectionAndSpinCount(&mCriticalSection, 0x00000400);
		if (res == false)
			_wassert(L"Create CriticalSection Failed", MAKE_WIDE(__FILE__), __LINE__);
	}

	~SCriticalSection() noexcept
	{
		DeleteCriticalSection(&mCriticalSection);
	}

	void lock() noexcept
	{
		EnterCriticalSection(&mCriticalSection);
	}

	void unlock() noexcept
	{
		LeaveCriticalSection(&mCriticalSection);
	}

	CRITICAL_SECTION mCriticalSection;
};

#pragma once

#include "Core/Macros/UtilMacros.h"

#include <stdint.h>

struct SCRTLibrary
{
	NORETURN static void Assert(const wchar_t* _message, const wchar_t* _file, uint32_t _line) noexcept;
};

#pragma once

#include <corecrt.h>

extern "C"
{
	_ACRTIMP void __cdecl _wassert(
		_In_z_ wchar_t const* _Message,
		_In_z_ wchar_t const* _File,
		_In_   unsigned       _Line
	);
}

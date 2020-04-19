#pragma once

#include "Template/TemplateUtil.h"

#include <stdint.h>
#include <type_traits>

using SWndProcFuncType = TSintPtrType(_stdcall)(HWND, uint32_t, TIntPtrType, TSintPtrType);

template<uint64_t _id> struct TUniqueHandleHelperStruct { int unused; };
template<uint64_t _id> using TUniqueHandleHelper = TUniqueHandleHelperStruct<_id>*;

//HINSTANCE is same as HMODULE
using HINSTANCE = TUniqueHandleHelper<0>;
using HMODULE	= TUniqueHandleHelper<0>;

using HANDLE	= TUniqueHandleHelper<1>;
using HWND		= TUniqueHandleHelper<2>;
using HEVENT	= TUniqueHandleHelper<3>;
using HICON		= TUniqueHandleHelper<4>;
using HCURSOR	= TUniqueHandleHelper<5>;
using HBRUSH	= TUniqueHandleHelper<6>;
using HMENU		= TUniqueHandleHelper<7>;

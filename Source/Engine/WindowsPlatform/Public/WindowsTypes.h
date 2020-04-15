#pragma once

#include <stdint.h>
#include <type_traits>

template<uint64_t _id> struct TUniqueHandleHelperStruct { int unused; };
template<uint64_t _id> using TUniqueHandleHelper = TUniqueHandleHelperStruct<_id>*;

using HANDLE = TUniqueHandleHelper<0>;

using HINSTANCE = TUniqueHandleHelper<1>;

//HINSTANCE is same as HMODULE
using HMODULE = TUniqueHandleHelper<1>;

using HWND = TUniqueHandleHelper<2>;

#pragma once

#include "Core/Util/Assert.h"
#include <format>

#define VERIFY_D3D_RETURN(_expr)													\
do {																				\
	HRESULT hr = (_expr);															\
	if(hr != S_OK)																	\
	{																				\
		OutputDebugStringW(std::format(L"HRESULT: [{0}]", hr).c_str());	\
		CHECK(#_expr);																\
	}																				\
} while(0)


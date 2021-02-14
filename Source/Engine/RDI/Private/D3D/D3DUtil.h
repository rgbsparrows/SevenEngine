#pragma once

#include "Core/Macros/Assert.h"

#define VERIFY_D3D_RETURN(_expr) CHECK("D3D ERROR" && ((_expr) == 0))

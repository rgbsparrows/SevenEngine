#pragma once

#include "Core/Util/Assert.h"

#define VERIFY_D3D_RETURN(_expr) CHECK("D3D ERROR" && ((_expr) == 0))

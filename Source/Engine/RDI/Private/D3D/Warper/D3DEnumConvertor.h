#pragma once

#include "RDI/RDIPixelFormat.h"
#include "RDI/RDIFunctionHelper.h"
#include "D3D/Warper/D3DImplWarperHelper.h"

DXGI_FORMAT ConvertPixelFormatToD3D(ERDIPixelFormat _pixelFormat) noexcept;
const char* ConvertShaderTargetToStr(ED3DShaderTarget _shaderTarget) noexcept;
DXGI_MODE_SCALING ConvertScalingModeToD3D(ERDIScalingMode _scalingMode) noexcept;

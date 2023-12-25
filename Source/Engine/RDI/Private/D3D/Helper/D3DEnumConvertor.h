#pragma once

#include "RDI/RDIPixelFormat.h"
#include "D3D/Helper/D3DHelper.h"
#include "RDI/RDIFunctionHelper.h"

#include "Core/Misc/PreWindowsApi.h"
#include <dxgi1_6.h>
#include "Core/Misc/PostWindowsApi.h"

DXGI_FORMAT ConvertPixelFormatToD3D(ERDIPixelFormat _pixelFormat) noexcept;
const char* ConvertShaderTargetToStr(ED3DShaderTarget _shaderTarget) noexcept;
const char* GetShaderEntryPoint(ED3DShaderTarget _shaderTarget) noexcept;
DXGI_MODE_SCALING ConvertScalingModeToD3D(ERDIScalingMode _scalingMode) noexcept;

#pragma once

#include "Core/Modules/ModuleInterface.h"

#include <vector>
#include <string_view>

__interface IRDIFactory;

__interface IRDIModuleInterface : IModuleInterface
{
	IRDIFactory* GetRDIFactory() noexcept;
};

IRDIModuleInterface* GetRDIModule() noexcept;

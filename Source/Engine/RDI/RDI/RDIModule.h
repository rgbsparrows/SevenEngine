#pragma once

#include "RDI/Interface/RDIFactory.h"
#include "Core/Modules/ModuleInterface.h"

#include <vector>
#include <string_view>

__interface IRDIModuleInterface : IModuleInterface
{
	IRDIFactory* CreateRDIFactory(std::wstring_view _rdiName) noexcept;
	IRDIFactory* GetRDIFactory(std::wstring_view _rdiName) noexcept;
	void ReleaseRDIFactory(IRDIFactory* _rdiFactory) noexcept;
};

IRDIModuleInterface* GetRDIModule() noexcept;

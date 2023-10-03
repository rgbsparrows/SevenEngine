#pragma once

#include "Core/Modules/ModuleInterface.h"

__interface IGMQuantModule : IModuleInterface
{
	bool Init() noexcept;
	void Clear() noexcept;
};

IGMQuantModule* GetGMQuantModule() noexcept;
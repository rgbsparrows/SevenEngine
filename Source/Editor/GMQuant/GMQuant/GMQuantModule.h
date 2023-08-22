#pragma once

#include "Core/Modules/ModuleInterface.h"

#include <functional>

class SQuantStrategyBase;

using SCreateQuantStrategyType = std::function<SQuantStrategyBase* ()>;

__interface IGMQuantModule : IModuleInterface
{
	bool Init() noexcept;
	void Clear() noexcept;
};

IGMQuantModule* GetGMQuantModule() noexcept;

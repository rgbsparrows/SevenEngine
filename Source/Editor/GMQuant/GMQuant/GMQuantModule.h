#pragma once

#include "Core/Modules/ModuleInterface.h"

#include <functional>

class SQuantStrategyBase;

using SCreateQuantStrategyType = std::function<SQuantStrategyBase* ()>;

__interface IGMQuantModule : IModuleInterface
{
	bool Init() noexcept;
	void Clear() noexcept;

	void RegisterQuantStrategy(SCreateQuantStrategyType&& _createQuantStrategy, const std::string& _strategyName) noexcept;
};

IGMQuantModule* GetGMQuantModule() noexcept;

#pragma once

#include "QuantType.h"
#include "Core/Modules/ModuleInterface.h"

#include <filesystem>

class SQuantStrategyBase;

//掘金量化模块
__interface IGMQuantCoreModule : IModuleInterface
{
	bool Init() noexcept;
	void Clear() noexcept;

	void StartupQuantitativeTerminal(const std::filesystem::path& _gmQuantTerminalPath) const noexcept;
	void SetUserToken(const std::string& _userToken) noexcept;
	void ExecuteBacktestQuantStrategy(SQuantStrategyBase* _strategy, const std::string& _strategyTokenName, const SBacktestConfig& _backtestConfig = SBacktestConfig()) noexcept;
	void ExecuteRealtimeQuantStrategy(SQuantStrategyBase* _strategy, const std::string& _strategyTokenName) noexcept;
};

IGMQuantCoreModule* GetGMQuantCoreModule() noexcept;

#pragma once

#include "Core/Modules/ModuleInterface.h"

#include <filesystem>

struct SBacktestConfig;
class SQuantStrategyBase;

//掘金量化模块
__interface IGMQuantCoreModule : IModuleInterface
{
	bool Init() noexcept;
	void Clear() noexcept;

	void StartupQuantitativeTerminal(const std::filesystem::path& _gmQuantTerminalPath) const noexcept;
	void InitQuantSystem(const std::string& _userToken) noexcept;
	void ExecuteBacktestQuantStrategy(SQuantStrategyBase* _strategy, const std::string& _strategyTokenName, const SBacktestConfig& _backtestConfig) noexcept;
	void ExecuteRealtimeQuantStrategy(SQuantStrategyBase* _strategy, const std::string& _strategyTokenName) noexcept;
};

IGMQuantCoreModule* GetGMQuantCoreModule() noexcept;

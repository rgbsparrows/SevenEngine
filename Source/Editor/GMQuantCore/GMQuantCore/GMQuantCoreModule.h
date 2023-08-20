#pragma once

#include "QuantType.h"
#include "Core/Modules/ModuleInterface.h"

#include <filesystem>

class SQuantStrategyBase;

//�������ģ��
__interface IGMQuantCoreModule : IModuleInterface
{
	bool Init() noexcept;
	void Clear() noexcept;

	void StartupQuantitativeTerminal(const std::filesystem::path& _gmQuantTerminalPath, const std::string& _userToken) noexcept;
	void ExecuteBacktestQuantStrategy(SQuantStrategyBase* _strategy, const std::string& _strategyTokenName, const SBacktestConfig& _backtestConfig = SBacktestConfig()) noexcept;
	void ExecuteRealtimeQuantStrategy(SQuantStrategyBase* _strategy, const std::string& _strategyTokenName) noexcept;

	// bool CheckQuantitaltiveTerinalState() const noexcept;
	// std::vector<SStockInfo> GetAllStocks() const noexcept;
};

IGMQuantCoreModule* GetGMQuantCoreModule() noexcept;
#pragma once

#include "GMQuantCore/QuantType.h"
#include "GMQuantCore/GMQuantCoreModule.h"

#include "gmapi.h"
#include <thread>
#include <functional>

class SGMQuantStrategyContextImpl;

class SGMQuantCoreModuleImpl : public IGMQuantCoreModule
{
public:
	bool Init() noexcept override;
	void Clear() noexcept override;

	void StartupQuantitativeTerminal(const std::filesystem::path& _gmQuantTerminalPath) const noexcept override;
	void SetUserToken(const std::string& _userToken) noexcept override;
	void ExecuteBacktestQuantStrategy(SQuantStrategyBase* _strategy, const std::string& _strategyTokenName, const SBacktestConfig& _backtestConfig) noexcept override;
	void ExecuteRealtimeQuantStrategy(SQuantStrategyBase* _strategy, const std::string& _strategyTokenName) noexcept override;

private:
	void QuantThreadMain() noexcept;

private:
	using SQuantTaskType = std::function<void(SGMQuantStrategyContextImpl*)>;

	std::thread mQuantThread;
	std::atomic_bool mRequireExit;
	std::mutex mQuantTasksMutex;

	std::vector<SQuantTaskType> mQuantTaskList;
};

SGMQuantCoreModuleImpl* GetGMQuantCoreModuleImpl() noexcept;

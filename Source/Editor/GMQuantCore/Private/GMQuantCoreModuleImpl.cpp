#include "Core/Misc/windowsEx.h"
#include "GMQuantCoreModuleImpl.h"
#include "GMQuantStrategyFramework.h"
#include "GMQuantCore/QuantStrategy.h"

#include <functional>

SGMQuantCoreModuleImpl* GGMQuantCoreModuleImpl = nullptr;

IGMQuantCoreModule* GetGMQuantCoreModule() noexcept
{
	return GGMQuantCoreModuleImpl;
}

SGMQuantCoreModuleImpl* GetGMQuantCoreModuleImpl() noexcept
{
	return GGMQuantCoreModuleImpl;
}

bool SGMQuantCoreModuleImpl::Init() noexcept
{
	mQuantThread = std::thread(std::mem_fn(&SGMQuantCoreModuleImpl::QuantThreadMain), this);
	return true;
}

void SGMQuantCoreModuleImpl::Clear() noexcept
{
	if (mQuantThread.joinable())
		mQuantThread.join();
}

void SGMQuantCoreModuleImpl::StartupQuantitativeTerminal(const std::filesystem::path& _gmQuantTerminalPath) const noexcept
{
	ShellExecuteW(nullptr, L"open", _gmQuantTerminalPath.c_str(), nullptr, nullptr, FALSE);
}

void SGMQuantCoreModuleImpl::InitQuantSystem(const std::string& _userToken) noexcept
{
	std::lock_guard lock(mQuantTasksMutex);

	mQuantTaskList.push_back(
		[=](SGMQuantStrategyContextImpl* _strategyFramework) {
			_strategyFramework->SetUserToken(_userToken);
		}
	);
}

void SGMQuantCoreModuleImpl::ExecuteBacktestQuantStrategy(SQuantStrategyBase* _strategy, const std::string& _strategyTokenName, const SBacktestConfig& _backtestConfig) noexcept
{
	if (_strategy == nullptr)
		return;

	std::lock_guard lock(mQuantTasksMutex);

	_strategy->SetStrategyExecuteState(EStrategyExecuteState::WaitingForExecute);
	mQuantTaskList.push_back(
		[=](SGMQuantStrategyContextImpl* _strategyFramework) {
			_strategyFramework->ExecuteBacktestStrategy(_strategy, _strategyTokenName, _backtestConfig);
		}
	);
}

void SGMQuantCoreModuleImpl::ExecuteRealtimeQuantStrategy(SQuantStrategyBase* _strategy, const std::string& _strategyTokenName) noexcept
{
	if (_strategy == nullptr)
		return;

	std::lock_guard lock(mQuantTasksMutex);

	_strategy->SetStrategyExecuteState(EStrategyExecuteState::WaitingForExecute);
	mQuantTaskList.push_back(
		[=](SGMQuantStrategyContextImpl* _strategyFramework) {
			_strategyFramework->ExecuteRealtimeStrategy(_strategy, _strategyTokenName);
		}
	);
}

void SGMQuantCoreModuleImpl::QuantThreadMain() noexcept
{
	SGMQuantStrategyContextImpl strategyFramework;

	while (mRequireExit == false)
	{
		std::vector<SQuantTaskType> quantTaskList;
		{
			std::lock_guard lock(mQuantTasksMutex);
			quantTaskList = mQuantTaskList;
		}

		for (auto& quantTask : quantTaskList)
			quantTask(&strategyFramework);

		std::this_thread::yield();
	}
}

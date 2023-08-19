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

REGIST_MODULE("GMQuantCoreModule", SGMQuantCoreModuleImpl)

bool SGMQuantCoreModuleImpl::Init() noexcept
{
	GGMQuantCoreModuleImpl = this;

	mQuantThread = std::thread(std::mem_fn(&SGMQuantCoreModuleImpl::QuantThreadMain), this);
	return true;
}

void SGMQuantCoreModuleImpl::Clear() noexcept
{
	mRequireExit = true;
	if (mQuantThread.joinable())
		mQuantThread.join();

	GGMQuantCoreModuleImpl = nullptr;
}

void SGMQuantCoreModuleImpl::StartupQuantitativeTerminal(const std::filesystem::path& _gmQuantTerminalPath, const std::string& _userToken) noexcept
{
	ShellExecuteW(nullptr, L"open", std::format(L"\"{}\"", _gmQuantTerminalPath.c_str()).c_str(), nullptr, nullptr, TRUE);

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

	_strategy->Reset();
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
	Thread::SetCurrentThreadName(u8"量化线程");

	SGMQuantStrategyContextImpl strategyFramework;

	while (mRequireExit == false)
	{
		std::vector<SQuantTaskType> quantTaskList;
		{
			std::lock_guard lock(mQuantTasksMutex);
			quantTaskList = mQuantTaskList;
			mQuantTaskList.clear();
		}

		for (auto& quantTask : quantTaskList)
			quantTask(&strategyFramework);

		std::this_thread::yield();
	}
}

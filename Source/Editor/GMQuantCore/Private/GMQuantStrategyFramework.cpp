#include "Core/Math/Math.h"
#include "GMQuantCore/QuantType.h"
#include "Core/Util/TemplateUtil.h"
#include "GMQuantStrategyFramework.h"
#include "GMQuantCore/QuantStrategy.h"

#pragma comment(lib, "gmsdk.lib")

std::chrono::system_clock::time_point SGMQuantStrategyContextImpl::GetEarliestBacktestTime() const noexcept
{
	return std::chrono::sys_days(std::chrono::year_month_day(std::chrono::year(2000) / std::chrono::month(1) / std::chrono::day(1)));
}

std::chrono::system_clock::time_point SGMQuantStrategyContextImpl::GetLatestBacktestTime() const noexcept
{
	return std::chrono::system_clock::now();
}

std::chrono::system_clock::time_point SGMQuantStrategyContextImpl::GetNowTime() noexcept
{
	return std::chrono::system_clock::from_time_t(static_cast<std::time_t>(now()));
}

void SGMQuantStrategyContextImpl::SetUserToken(const std::string& _userToken) noexcept
{
	set_token(_userToken.c_str());
}

void SGMQuantStrategyContextImpl::ExecuteBacktestStrategy(SQuantStrategyBase* _strategy, const std::string& _strategyId, const SBacktestConfig& _backtestConfig) noexcept
{
	if (_strategy == nullptr)
		return;

	std::chrono::system_clock::time_point startTime = Math::Max(_backtestConfig.mStartTime, GetEarliestBacktestTime());
	std::chrono::system_clock::time_point endTime = Math::Min(_backtestConfig.mEndTime, GetLatestBacktestTime());

	mCurrentStrategy = _strategy;
	mIsBacktestMode = true;

	set_backtest_config(
		FormatTime_yy_mm_dd_hh_mm_ss(startTime).c_str(),
		FormatTime_yy_mm_dd_hh_mm_ss(endTime).c_str(),
		_backtestConfig.mInitialCash,
		_backtestConfig.mTransactionRatio,
		_backtestConfig.mCommissionRatio,
		_backtestConfig.mSlippageRatio,
		EnumToInt(_backtestConfig.mAdjustMode)
	);

	set_mode(MODE_BACKTEST);
	set_strategy_id(_strategyId.c_str());

	run();
}

void SGMQuantStrategyContextImpl::ExecuteRealtimeStrategy(SQuantStrategyBase* _strategy, const std::string& _strategyId) noexcept
{
	if (_strategy == nullptr)
		return;

	mCurrentStrategy = _strategy;
	mIsBacktestMode = false;

	set_mode(MODE_LIVE);
	set_strategy_id(_strategyId.c_str());

	run();
}

void SGMQuantStrategyContextImpl::on_init()
{
	if (mCurrentStrategy == nullptr)
		return;

	for (std::chrono::hh_mm_ss hms : mCurrentStrategy->GetScheduleTimeList())
		schedule("1d", FormatTime_hh_mm_ss(hms).c_str());

	if (mIsBacktestMode == false)
		subscribe("SHSE.000001", "tick");

	mCurrentStrategy->SetStrategyExecuteState(EStrategyExecuteState::Executing);
	mCurrentStrategy->Init(this);
}

void SGMQuantStrategyContextImpl::on_tick(Tick*)
{
	if (mCurrentStrategy == nullptr)
		return;

	if (mCurrentStrategy->IsRequireExit())
		stop();

	mCurrentStrategy->Tick(this);
}

void SGMQuantStrategyContextImpl::on_schedule(const char*, const char*)
{
	if (mCurrentStrategy == nullptr)
		return;

	if (mCurrentStrategy->IsRequireExit())
		stop();

	mCurrentStrategy->Schedule(this);
}

void SGMQuantStrategyContextImpl::on_backtest_finished(Indicator*)
{
	if (mCurrentStrategy == nullptr)
		return;

	unsubscribe("SHSE.000001", "tick");

	mCurrentStrategy->Stop(this);
	mCurrentStrategy->SetStrategyExecuteState(EStrategyExecuteState::Executed);

	mCurrentStrategy = nullptr;
	mIsBacktestMode = true;
}

void SGMQuantStrategyContextImpl::on_stop()
{
	if (mCurrentStrategy == nullptr)
		return;

	unsubscribe("SHSE.000001", "tick");

	mCurrentStrategy->Stop(this);
	mCurrentStrategy->SetStrategyExecuteState(EStrategyExecuteState::Executed);

	mCurrentStrategy = nullptr;
	mIsBacktestMode = true;
}

std::string SGMQuantStrategyContextImpl::FormatTime_yy_mm_dd_hh_mm_ss(std::chrono::system_clock::time_point _time) const noexcept
{
	std::chrono::year_month_day ymd = std::chrono::floor<std::chrono::days>(_time);
	std::chrono::hh_mm_ss<std::chrono::seconds> hms(std::chrono::floor<std::chrono::seconds>(_time) - std::chrono::floor<std::chrono::days>(_time));

	return std::format("{:%F} {:%T}", ymd, hms);
}

std::string SGMQuantStrategyContextImpl::FormatTime_yy_mm_dd(std::chrono::year_month_day _time) const noexcept
{
	return std::format("{:%F}", _time);
}

std::string SGMQuantStrategyContextImpl::FormatTime_hh_mm_ss(std::chrono::hh_mm_ss<std::chrono::seconds> _time) const noexcept
{
	return std::format("{:%T}", _time);
}

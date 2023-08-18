#include "Core/Math/Math.h"
#include "GMQuantCore/QuantType.h"
#include "Core/Util/TemplateUtil.h"
#include "GMQuantStrategyFramework.h"
#include "GMQuantCore/QuantStrategy.h"
#include "GMQuantCore/Util/QuantMisc.h"

#pragma comment(lib, "gmsdk.lib")

std::chrono::system_clock::time_point SGMQuantStrategyContextImpl::GetEarliestBacktestTime() noexcept
{
	return std::chrono::sys_days(std::chrono::year_month_day(std::chrono::year(1991) / std::chrono::month(1) / std::chrono::day(1))) - std::chrono::hours(8);
}

std::chrono::system_clock::time_point SGMQuantStrategyContextImpl::GetLatestBacktestTime() noexcept
{
	return std::chrono::system_clock::now();
}

std::chrono::system_clock::time_point SGMQuantStrategyContextImpl::GetNowTime() noexcept
{
	return std::chrono::system_clock::from_time_t(static_cast<std::time_t>(now()));
}

std::vector<std::string> SGMQuantStrategyContextImpl::GetAllStockSymbols(std::chrono::system_clock::time_point _time) noexcept
{
	std::vector<std::string> symbolList;
	DataArray<SymbolContent>* res = get_symbols_by_date(1010, 101001, nullptr, nullptr, true, false, FormatTime_yy_mm_dd_hh_mm_ss(_time).c_str());
	if (res->status() == 0)
		for (int i = 0; i != res->count(); ++i)
			symbolList.push_back(res->at(i).info.symbol);

	Sort(symbolList);
	DedupForSortedRange(symbolList);

	return symbolList;
}

std::vector<STick> SGMQuantStrategyContextImpl::GetHistoryTick(const std::string& _symbol, std::chrono::system_clock::time_point _startTime, std::chrono::system_clock::time_point _endTime, EAdjustMode _adjustMode) noexcept
{
	DataArray<Tick>* res = history_ticks(_symbol.c_str(), FormatTime_yy_mm_dd_hh_mm_ss(_startTime).c_str(), FormatTime_yy_mm_dd_hh_mm_ss(_endTime).c_str(), EnumToInt(_adjustMode));

	return ConvertToTickList(res);
}

std::vector<STick> SGMQuantStrategyContextImpl::GetHistoryNTick(const std::string& _symbol, uint64_t _n, std::chrono::system_clock::time_point _endTime, EAdjustMode _adjustMode) noexcept
{
	DataArray<Tick>* res = history_ticks_n(_symbol.c_str(), static_cast<int>(_n), FormatTime_yy_mm_dd_hh_mm_ss(_endTime).c_str(), EnumToInt(_adjustMode));

	return ConvertToTickList(res);
}

std::vector<STick> SGMQuantStrategyContextImpl::GetLastestNTick(const std::string& _symbol, uint64_t _n, EAdjustMode _adjustMode)
{
	return GetHistoryNTick(_symbol, _n, GetNowTime(), _adjustMode);
}

std::vector<SBar> SGMQuantStrategyContextImpl::GetHistoryBar(const std::string& _symbol, std::chrono::system_clock::time_point _startTime, std::chrono::system_clock::time_point _endTime, EFrequency _frequency, EAdjustMode _adjustMode) noexcept
{
	DataArray<Bar>* res = history_bars(_symbol.c_str(), FormatFrequency(_frequency).c_str(), FormatTime_yy_mm_dd_hh_mm_ss(_startTime).c_str(), FormatTime_yy_mm_dd_hh_mm_ss(_endTime).c_str(), EnumToInt(_adjustMode));

	return ConvertToBarList(res);
}

std::vector<SBar> SGMQuantStrategyContextImpl::GetHistoryNBar(const std::string& _symbol, uint64_t _n, std::chrono::system_clock::time_point _endTime, EFrequency _frequency, EAdjustMode _adjustMode) noexcept
{
	DataArray<Bar>* res = history_bars_n(_symbol.c_str(), FormatFrequency(_frequency).c_str(), static_cast<int>(_n), FormatTime_yy_mm_dd_hh_mm_ss(_endTime).c_str(), EnumToInt(_adjustMode));

	return ConvertToBarList(res);
}

std::vector<SBar> SGMQuantStrategyContextImpl::GetLastestNBar(const std::string& _symbol, uint64_t _n, EFrequency _frequency, EAdjustMode _adjustMode) noexcept
{
	return GetHistoryNBar(_symbol, _n, GetNowTime(), _frequency, _adjustMode);
}

void SGMQuantStrategyContextImpl::BuyVolume(const std::string& _symbol, uint64_t _volume, float _price) noexcept
{
	order_volume(_symbol.c_str(), static_cast<int>(_volume), OrderSide_Buy, OrderType_Limit, PositionEffect_Open, _price);
}

void SGMQuantStrategyContextImpl::SellVolume(const std::string& _symbol, uint64_t _volume, float _price) noexcept
{
	order_volume(_symbol.c_str(), static_cast<int>(_volume), OrderSide_Sell, OrderType_Limit, PositionEffect_Close, _price);
}

SCash SGMQuantStrategyContextImpl::GetCash() noexcept
{
	DataArray<Cash>* cashList = get_cash();
	if (cashList && cashList->status() == 0 && cashList->count() != 0)
	{
		SCash cash;
		const Cash& rawCash = cashList->at(0);
		cash.mBalance = rawCash.balance;
		cash.mMarketValue = rawCash.market_value;
		return cash;
	}

	return SCash();
}

std::vector<SPosition> SGMQuantStrategyContextImpl::GetPositionList() noexcept
{
	DataArray<Position>* res = get_position();

	return ConvertToPositionList(res);
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

	std::vector<std::chrono::hh_mm_ss<std::chrono::seconds>> scheduleTimeList = mCurrentStrategy->GetScheduleTimeList();
	for (const auto& hms : scheduleTimeList)
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

std::string SGMQuantStrategyContextImpl::FormatFrequency(EFrequency _frequency) const noexcept
{
	switch (_frequency)
	{
	case EFrequency::FREQUENCY_1MIN: return "1m";
	case EFrequency::FREQUENCY_5MIN: return "5m";
	case EFrequency::FREQUENCY_15MIN: return "15m";
	case EFrequency::FREQUENCY_30MIN: return "30m";
	case EFrequency::FREQUENCY_60MIN: return "60m";
	case EFrequency::FREQUENCY_1DAY: return "1d";
	default: return "1d";
	}
}

std::vector<STick> SGMQuantStrategyContextImpl::ConvertToTickList(DataArray<Tick>* _rawTickList) noexcept
{
	std::vector<STick> tickList;

	if (_rawTickList && _rawTickList->status() == 0)
	{
		for (int i = 0; i != _rawTickList->count(); ++i)
		{
			const Tick& rawTick = _rawTickList->at(i);
			STick tick;
			tick.mTime = std::chrono::system_clock::from_time_t(static_cast<std::time_t>(rawTick.created_at));
			tick.mPrice = rawTick.price;
			tick.mOpen = rawTick.open;
			tick.mHigh = rawTick.high;
			tick.mLow = rawTick.low;
			tick.mCumVolume = static_cast<uint64_t>(rawTick.cum_volume);
			tick.mCumAmount = rawTick.cum_amount;
			tick.mVolume = rawTick.last_volume;
			tick.mAmount = rawTick.last_amount;

			for (size_t j = 0; j != DEPTH_OF_QUOTE; ++j)
			{
				SQuote bidQuote;
				SQuote askQuote;

				bidQuote.mPrice = rawTick.quotes[j].bid_price;
				bidQuote.mVolume = rawTick.quotes[j].bid_volume;
				askQuote.mPrice = rawTick.quotes[j].ask_price;
				askQuote.mVolume = rawTick.quotes[j].ask_volume;

				tick.mBidQuoteList.push_back(bidQuote);
				tick.mAskQuoteList.push_back(askQuote);
			}
			tickList.push_back(tick);
		}
	}

	return tickList;
}

std::vector<SBar> SGMQuantStrategyContextImpl::ConvertToBarList(DataArray<Bar>* _rawBarLists) noexcept
{
	std::vector<SBar> barList;

	if (_rawBarLists && _rawBarLists->status() == 0)
	{
		for (int i = 0; i != _rawBarLists->count(); ++i)
		{
			const Bar& rawBar = _rawBarLists->at(i);
			SBar bar;
			bar.mBob = std::chrono::system_clock::from_time_t(static_cast<std::time_t>(rawBar.bob));
			bar.mEob = std::chrono::system_clock::from_time_t(static_cast<std::time_t>(rawBar.eob));
			bar.mOpen = rawBar.open;
			bar.mHigh = rawBar.high;
			bar.mLow = rawBar.low;
			bar.mClose = rawBar.close;
			bar.mVolume = static_cast<uint64_t>(rawBar.volume);
			bar.mAmount = rawBar.amount;
			barList.push_back(bar);
		}
	}

	return barList;
}

std::vector<SPosition> SGMQuantStrategyContextImpl::ConvertToPositionList(DataArray<Position>* _rawPositionList) noexcept
{
	std::vector<SPosition> positionList;

	if (_rawPositionList && _rawPositionList->status() == 0)
	{
		for (int i = 0; i != _rawPositionList->count(); ++i)
		{
			const Position& rawPosition = _rawPositionList->at(i);
			SPosition position;
			position.mSymbol = rawPosition.symbol;
			position.mMarketValue = rawPosition.market_value;

			position.mVolumeToday = static_cast<uint64_t>(rawPosition.volume_today);
			position.mVolume = static_cast<uint64_t>(rawPosition.volume);

			position.mPrice = rawPosition.price;
			position.mCost = rawPosition.cost;

			position.mCreateTime = std::chrono::system_clock::from_time_t(static_cast<std::time_t>(rawPosition.created_at));
			position.mUpdateTime = std::chrono::system_clock::from_time_t(static_cast<std::time_t>(rawPosition.updated_at));

			positionList.push_back(position);
		}
	}

	return positionList;
}

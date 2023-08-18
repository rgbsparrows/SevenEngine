#pragma once

#include <chrono>
#include <string>

enum class EAdjustMode
{
	ADJUST_NONE,
	ADJUST_FORWARD,
	ADJUST_BACKWARD,
};

struct SBacktestConfig
{
	// 起始时间
	std::chrono::system_clock::time_point mStartTime = std::chrono::system_clock::time_point::min();
	// 终止时间
	std::chrono::system_clock::time_point mEndTime = std::chrono::system_clock::time_point::max();

	// 初始现金
	float mInitialCash = 1000000.0f;
	// 成交比例
	float mTransactionRatio = 1.f;
	// 佣金比例
	float mCommissionRatio = 0.0002f;
	// 滑点比例
	float mSlippageRatio = 0.0001f;
	// 复权模式
	EAdjustMode mAdjustMode = EAdjustMode::ADJUST_BACKWARD;
};

enum class EFrequency
{
	FREQUENCY_1MIN,
	FREQUENCY_5MIN,
	FREQUENCY_15MIN,
	FREQUENCY_30MIN,
	FREQUENCY_60MIN,
	FREQUENCY_1DAY,
};

struct SQuote
{
	double mPrice = 0.f; // 价格
	uint64_t mVolume = 0; // 单量
};

struct STick
{
	std::chrono::system_clock::time_point mTime; // 当前时间
	double mPrice = 0.f; // 当前价格
	double mOpen = 0.f; // 开盘价
	double mHigh = 0.f; // 最高价
	double mLow = 0.f; // 最低价

	uint64_t mCumVolume = 0; // 总成交量
	double mCumAmount; // 总成交额
	double mVolume = 0.f; // 当前成交量
	double mAmount = 0.f; // 当前成交额

	std::vector<SQuote> mBidQuoteList; // 买盘
	std::vector<SQuote> mAskQuoteList; // 卖盘
};

struct SBar
{
	std::chrono::system_clock::time_point mBob; // 开始时间
	std::chrono::system_clock::time_point mEob; // 结束时间

	double mOpen = 0.f; // 开盘价
	double mHigh = 0.f; // 最高价
	double mLow = 0.f; // 最低价
	double mClose = 0.f; // 收盘价

	uint64_t mVolume = 0; // 成交量
	double mAmount = 0.f; // 成交额
};

enum class EBoard
{
	BOARD_A, // A股主板
	BOARD_B, // B股主板
	BOARD_CY, // 创业板
	BOARD_KC, // 科创板
	BOARD_ETF, // ETF
};

struct SStockInfo
{
	EBoard mBoard; // 板块
	std::string mSymbolName; // 股票名称
	std::string mAbbr; // 股票简称
	double mPriceTick; // 价格最小变动单位
	std::chrono::system_clock::time_point mListDate; // 上市日期
	std::chrono::system_clock::time_point mDelistDate; // 退市日期

	bool mIsSuspended; // 是否停牌
	bool mIsST; // 是否ST
	float mAdjFactor; // 复权因子
};

struct SCash
{
	double mBalance = 0.f; // 余额
	double mMarketValue = 0.f; // 持仓市值
};

struct SPosition
{
	std::string mSymbol; // 股票代码
	double mMarketValue; // 持仓市值

	uint64_t mVolumeToday; // 今日持仓量
	uint64_t mVolume; // 持仓量
	
	double mPrice; // 现价
	double mCost; // 成本
	
	std::chrono::system_clock::time_point mCreateTime; // 建仓时间
	std::chrono::system_clock::time_point mUpdateTime; // 更新时间
};

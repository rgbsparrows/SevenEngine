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

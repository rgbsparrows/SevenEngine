#pragma once

#include "Core/Math/Math.h"

#include <vector>

struct SMACD
{
	float mDiff;
	float mDea;
	float mMacd;
};

std::vector<float> CalcMa(const std::vector<float>& _data, size_t _period) noexcept;
std::vector<float> CalcEma(const std::vector<float>& _data, size_t _period) noexcept;

std::vector<SMACD> CalcMacd(const std::vector<float>& _data, size_t _shortPeriod, size_t _longPeriod, size_t _signalPeriod) noexcept;
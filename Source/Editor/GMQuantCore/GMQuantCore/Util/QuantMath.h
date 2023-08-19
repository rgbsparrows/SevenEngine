#pragma once

#include "Core/Math/Math.h"

#include <vector>

struct SMACD
{
	double mDiff;
	double mDea;
	double mMacd;
};

std::vector<double> CalcMa(const std::vector<double>& _data, size_t _period) noexcept;
std::vector<double> CalcEma(const std::vector<double>& _data, size_t _period) noexcept;

std::vector<SMACD> CalcMacd(const std::vector<double>& _data, size_t _shortPeriod, size_t _longPeriod, size_t _signalPeriod) noexcept;

uint64_t CalcMaxPurchaseCount(double money, double _price, double _commission) noexcept;

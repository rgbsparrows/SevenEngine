#pragma once

#include "Core/Math/Math.h"
#include "GMQuantCore/QuantType.h"

#include <vector>

struct SMACD
{
	double mDiff;
	double mDea;
	double mMacd;
};

enum class EZTTrend
{
	Up,
	Down,
	Consolidation,
	ConsolidationUp,
	ConsolidationDown,
};

//��(����)
struct SZTPath
{
	EZTTrend GetTrend() const
	{
		if (mPriceStart < mPriceEnd)
			return EZTTrend::Up;
		else if (mPriceStart > mPriceEnd)
			return EZTTrend::Down;
		else
			return EZTTrend::Consolidation;
	}

	size_t mBarStart = 0;
	size_t mBarEnd = 0;

	double mPriceStart = 0.f;
	double mPriceEnd = 0.f;
};

// ����(����)
struct SZTPivot
{
	size_t mBarStart = 0;
	size_t mBarEnd = 0;

	double mPriceHigh = 0.f;
	double mPriceLow = 0.f;
};

//�߶�(����)
struct SZTPolyLine
{
	EZTTrend GetTrend() const
	{
		if (mPriceStart < mPriceEnd)
			return mPivotList.size() >= 2 ? EZTTrend::Up : EZTTrend::ConsolidationUp;
		else if (mPriceStart > mPriceEnd)
			return mPivotList.size() >= 2 ? EZTTrend::Down : EZTTrend::ConsolidationDown;

		return EZTTrend::Consolidation;
	}

	size_t mBarStart = 0;
	size_t mBarEnd = 0;

	size_t mPathStart = 0;
	size_t mPathEnd = 0;

	double mPriceStart = 0.f;
	double mPriceEnd = 0.f;

	std::vector<SZTPivot> mPivotList;
};

// �������Ƽ���
struct SZTLevel
{
	std::vector<SZTPath> mPathList;
	std::vector<SZTPolyLine> mPolyLineList;
};

// ��������
struct SZTContent
{
	std::vector<SBar> mRawBarList;
	std::vector<SZTLevel> mLevelList;//���Ƽ���
};

std::vector<double> CalcMa(const std::vector<double>& _data, size_t _period) noexcept;
std::vector<double> CalcEma(const std::vector<double>& _data, size_t _period) noexcept;

std::vector<SMACD> CalcMacd(const std::vector<double>& _data, size_t _shortPeriod, size_t _longPeriod, size_t _signalPeriod) noexcept;

uint64_t CalcMaxPurchaseCount(double money, double _price, double _commission) noexcept;

SZTContent CalcZT(const std::vector<SBar>& _barList, bool _useHachure = true) noexcept;

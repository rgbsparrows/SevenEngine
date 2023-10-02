#include "GMQuantCore/Util/QuantMath.h"

std::vector<double> CalcMa(const std::vector<double>& _data, size_t _period) noexcept
{
	if (_period <= 1 || _data.empty()) return _data;

	std::vector<double> ma(_data.size(), 0.0f);

	double sum = 0.0f;
	for (size_t i = 0; i < _period && i < _data.size(); ++i)
	{
		sum += _data[i];
		ma[i] = sum / (i + 1);
	}

	for (size_t i = _period; i < _data.size(); ++i)
	{
		sum += _data[i] - _data[i - _period];
		ma[i] = sum / _period;
	}

	return ma;
}

std::vector<double> CalcEma(const std::vector<double>& _data, size_t _period) noexcept
{
	if (_period <= 1 || _data.empty()) return _data;

	std::vector<double> ema(_data.size(), 0.0f);

	ema[0] = _data[0];

	for (size_t i = 1; i < _data.size(); ++i)
		ema[i] = (_data[0] * 2 + ema[i - 1] * (_period - 1)) / _period;

	return ema;
}

std::vector<SMACD> CalcMacd(const std::vector<double>& _data, size_t _shortPeriod, size_t _longPeriod, size_t _signalPeriod) noexcept
{
	if (_shortPeriod <= 1 || _longPeriod <= 1 || _signalPeriod <= 1 || _data.empty())
		return std::vector<SMACD>(_data.size(), { 0.0f, 0.0f, 0.0f });

	std::vector<double> shortEma = CalcEma(_data, _shortPeriod);
	std::vector<double> longEma = CalcEma(_data, _longPeriod);

	std::vector<SMACD> macd(_data.size(), { 0.0f, 0.0f, 0.0f });
	std::vector<double> diff(_data.size(), 0.f);
	for (size_t i = 0; i < _data.size(); ++i)
	{
		macd[i].mDiff = shortEma[i] - longEma[i];
		diff[i] = macd[i].mDiff;
	}


	std::vector<double> signal = CalcEma(diff, _signalPeriod);
	for (size_t i = 0; i < _data.size(); ++i)
	{
		macd[i].mDea = signal[i];
		macd[i].mMacd = (macd[i].mDiff - macd[i].mDea) * 2;
	}

	return macd;
}

uint64_t CalcMaxPurchaseCount(double money, double _price, double _commission) noexcept
{
	double count = money / ((1 + _commission) * _price) / 100;
	return static_cast<uint64_t>(count) * 100;
}

namespace
{
	double GetBarHigh(const SBar& _bar, bool _useHachure) noexcept
	{
		return _useHachure ? _bar.mHigh : std::max(_bar.mOpen, _bar.mClose);
	}

	double GetBarLow(const SBar& _bar, bool _useHachure) noexcept
	{
		return _useHachure ? _bar.mLow : std::min(_bar.mOpen, _bar.mClose);
	}

	EZTTrend GetBarTrend(const SBar& _left, const SBar& _right, bool _useHachure) noexcept
	{
		if (GetBarHigh(_right, _useHachure) > GetBarHigh(_left, _useHachure) && GetBarLow(_right, _useHachure) > GetBarLow(_left, _useHachure))
			return EZTTrend::Up;

		if (GetBarHigh(_right, _useHachure) < GetBarHigh(_left, _useHachure) && GetBarLow(_right, _useHachure) < GetBarLow(_left, _useHachure))
			return EZTTrend::Down;

		return EZTTrend::Consolidation;
	}

	std::vector<SZTPath> CalcPathList(const std::vector<SBar>& _barList, bool _useHachure) noexcept
	{
		if (_barList.size() < 2)
			return std::vector<SZTPath>();

		std::vector<SZTPath> pathList;

		for (size_t i = 1; i != _barList.size(); ++i)
		{
			EZTTrend trend = GetBarTrend(_barList[i - 1], _barList[i], _useHachure);

			if (pathList.empty())
			{
				if (trend == EZTTrend::Consolidation)
					continue;
				double low = DBL_MAX;
				double high = DBL_MIN;

				for (size_t j = 0; j != i; ++j)
				{
					low = std::min(low, GetBarLow(_barList[j], _useHachure));
					high = std::max(high, GetBarHigh(_barList[j], _useHachure));
				}

				SZTPath newPath = SZTPath{ 0, i, 0, 0 };

				switch (trend)
				{
				case EZTTrend::Up:
					newPath.mPriceStart = low;
					newPath.mPriceEnd = GetBarHigh(_barList[i], _useHachure);
					break;
				case EZTTrend::Down:
					newPath.mPriceStart = high;
					newPath.mPriceEnd = GetBarLow(_barList[i], _useHachure);
					break;
				}
				pathList.push_back(newPath);
			}
			else if (trend != EZTTrend::Consolidation && pathList.back().GetTrend() != trend)
			{
				SZTPath lastPath = pathList.back();
				SZTPath newPath = SZTPath{ lastPath.mBarEnd, i, lastPath.mPriceEnd, 0 };

				switch (trend)
				{
				case EZTTrend::Up:
					newPath.mPriceEnd = GetBarHigh(_barList[i], _useHachure);
					break;
				case EZTTrend::Down:
					newPath.mPriceEnd = GetBarLow(_barList[i], _useHachure);
					break;
				}

				pathList.push_back(newPath);
			}
			else
			{
				pathList.back().mBarEnd = i;

				switch (trend)
				{
				case EZTTrend::Up:
					pathList.back().mPriceEnd = std::max(pathList.back().mPriceEnd, GetBarHigh(_barList[i], _useHachure));
					break;
				case EZTTrend::Down:
					pathList.back().mPriceEnd = std::min(pathList.back().mPriceEnd, GetBarLow(_barList[i], _useHachure));
					break;
				}
			}
		}

		if (pathList.size() > 1)
		{
			std::vector<SZTPath> fixedPathList;
			fixedPathList.push_back(pathList[0]);
			for (size_t i = 1; i != pathList.size(); ++i)
			{
				if (pathList[i].GetTrend() == fixedPathList.back().GetTrend())
				{
					fixedPathList.back().mBarEnd = pathList[i].mBarEnd;

					switch (pathList[i].GetTrend())
					{
					case EZTTrend::Up:
						fixedPathList.back().mPriceEnd = std::max(fixedPathList.back().mPriceEnd, pathList[i].mPriceEnd);
						break;
					case EZTTrend::Down:
						fixedPathList.back().mPriceEnd = std::min(fixedPathList.back().mPriceEnd, pathList[i].mPriceEnd);
						break;
					}
				}
				else
				{
					fixedPathList.push_back(pathList[i]);
				}
			}

			return fixedPathList;
		}

		return pathList;
	}

	EZTTrend GetPathTrend(const SZTPath& _left, const SZTPath& _right) noexcept
	{
		if (_right.mPriceStart > _left.mPriceStart && _right.mPriceEnd > _left.mPriceEnd)
			return EZTTrend::Up;

		if (_right.mPriceStart < _left.mPriceStart && _right.mPriceEnd < _left.mPriceEnd)
			return EZTTrend::Down;

		return EZTTrend::Consolidation;
	}

	std::vector<SZTPolyLine> CalcPolylineList(const std::vector<SZTPath>& _pathList) noexcept
	{
		if (_pathList.size() < 3)
			return std::vector<SZTPolyLine>();

		std::vector<SZTPolyLine> polyLineList;

		for (size_t i = 2; i != _pathList.size(); ++i)
		{
			if (_pathList[i].GetTrend() == GetPathTrend(_pathList[i - 2], _pathList[i]))
			{
				SZTPolyLine newPolyLine;
				newPolyLine.mBarStart = _pathList[0].mBarStart;
				newPolyLine.mBarEnd = _pathList[i].mBarEnd;

				newPolyLine.mPathStart = 0;
				newPolyLine.mPathEnd = i;

				newPolyLine.mPriceStart = _pathList[0].mPriceStart;
				newPolyLine.mPriceEnd = _pathList[i].mPriceEnd;

				polyLineList.push_back(newPolyLine);
				break;
			}
		}

		if (polyLineList.empty())
			return std::vector<SZTPolyLine>();

		for (size_t i = polyLineList[0].mPathEnd + 2; i < _pathList.size(); i += 2)
		{
			EZTTrend trend = GetPathTrend(_pathList[i - 2], _pathList[i]);

			if (trend == EZTTrend::Consolidation ||
				(trend == EZTTrend::Up && (polyLineList.back().mPriceEnd > polyLineList.back().mPriceStart)) ||
				(trend == EZTTrend::Down && (polyLineList.back().mPriceEnd < polyLineList.back().mPriceStart))
				)
			{
				polyLineList.back().mBarEnd = _pathList[i].mBarEnd;
				polyLineList.back().mPathEnd = i;
				polyLineList.back().mPriceEnd = _pathList[i].mPriceEnd;
			}
			else
			{
				SZTPolyLine newPolyLine;
				newPolyLine.mBarStart = _pathList[i - 1].mBarStart;
				newPolyLine.mBarEnd = _pathList[i].mBarEnd;

				newPolyLine.mPathStart = i - 1;
				newPolyLine.mPathEnd = i;

				newPolyLine.mPriceStart = _pathList[i - 1].mPriceStart;
				newPolyLine.mPriceEnd = _pathList[i - 1].mPriceEnd;

				polyLineList.push_back(newPolyLine);

				i = i - 1;
			}
		}

		// 在最后一笔为非关键笔时会进入此分支
		if (polyLineList.back().mPathEnd != _pathList.size() - 1)
		{
			polyLineList.back().mBarEnd = _pathList.back().mBarEnd;
			polyLineList.back().mPathEnd = _pathList.size() - 1;
		}

		return polyLineList;
	}
}


SZTContent CalcZT(const std::vector<SBar>& _barList, bool _useHachure) noexcept
{
	SZTContent ztContent;

	if (_barList.empty()) return ztContent;

	ztContent.mRawBarList = _barList;

	ztContent.mLevelList.push_back(SZTLevel());

	ztContent.mLevelList.back().mPathList = CalcPathList(_barList, _useHachure);
	ztContent.mLevelList.back().mPolyLineList = CalcPolylineList(ztContent.mLevelList.back().mPathList);

	//// First Level
	//{
	//	std::vector<SBar> mergedBar = CalcMergedBarList(_barList, _useHachure);

	//	if (mergedBar.size() <= 1)
	//		return;

	//	ztContent.mLevelList.push_back(SZTLevel());

	//	SZTLevel& ztLevel = ztContent.mLevelList.back();

	//	ztLevel.mPathList = CalcPathList(mergedBar);
	//	ztLevel.mPolyLineList = CalcPolyLineList(ztLevel.mPathList);
	//}

	//while (ztContent.mLevelList.back().mPolyLineList.size() > 1)
	//{

	//}

	return ztContent;
}

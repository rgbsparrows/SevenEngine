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

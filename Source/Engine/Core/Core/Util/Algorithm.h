#pragma once
#include <algorithm>
#include <vector>

template<typename _rangeType>
concept CRange = requires(_rangeType & _range) { std::begin(_range); std::end(_range); ++std::begin(_range); };

template<typename _rangeType>
concept CSwapableRange = CRange<_rangeType> && requires(_rangeType & _range) { std::iter_swap(_range.begin(), _range.begin()); };

template<typename _rangeType>
concept CEraseableRange = CRange<_rangeType> && requires(_rangeType & _range) { _range.erase(_range.begin()); };

template<CRange _rangeType, typename _valueType>
constexpr inline auto Find(_rangeType& _range, const _valueType& _value) noexcept
{
	return std::find(std::begin(_range), std::end(_range), _value);
}

template<CRange _rangeType, typename _predType>
constexpr inline auto FindIf(_rangeType& _range, _predType _pred) noexcept
{
	return std::find_if(std::begin(_range), std::end(_range), _pred);
}

template<CRange _rangeType, typename _valueType>
constexpr inline size_t FindIndex(_rangeType& _range, const _valueType& _value) noexcept
{
	auto it = std::find(std::begin(_range), std::end(_range), _value);
	return std::distance(std::begin(_range), it);
}

template<CRange _rangeType, typename _predType>
constexpr inline size_t FindIndexIf(_rangeType& _range, _predType _pred) noexcept
{
	auto it = std::find_if(std::begin(_range), std::end(_range), _pred);
	return std::distance(std::begin(_range), it);
}

template<CRange _rangeType, typename _valueType>
constexpr inline auto Has(_rangeType& _range, const _valueType& _value) noexcept
{
	return Find(_range, _value) == std::end(_range);
}

template<CRange _rangeType, typename _predType>
constexpr inline auto HasIf(_rangeType& _range, _predType _pred) noexcept
{
	return FindIf(_range, _pred) == std::end(_range);
}

template<CSwapableRange _rangeType>
constexpr inline auto Sort(_rangeType& _range) noexcept
{
	std::sort(_range.begin(), _range.end());
}

template<CSwapableRange _rangeType, typename _predType>
constexpr inline auto Sort(_rangeType& _range, _predType _pred) noexcept
{
	std::sort(_range.begin(), _range.end(), _pred);
}

template<CEraseableRange _rangeType>
constexpr inline auto DedupForSortedRange(_rangeType& _range) noexcept
{
	auto it = _range.begin();
	if (it == _range.end() || std::next(it) == _range.end())
		return;

	for (auto it = _range.begin(); std::next(it) != _range.end(); void())
	{
		if (*it == *std::next(it))
			it = _range.erase(it);
		else
			++it;
	}
}

template<CRange _rangeType, typename _valueType>
constexpr inline auto Append(_rangeType& _dstRange, const _valueType& _value) requires requires (_rangeType& _dstRange, const _valueType& _value) { { _dstRange.insert(_dstRange.begin(), _value) }; }
{
	_dstRange.insert(_dstRange.begin(), _value);
}

template<CRange _rangeType, CRange _sourceRangeType>
constexpr inline auto Append(_rangeType& _dstRange, const _sourceRangeType& _source) requires requires (_rangeType& _dstRange, const _sourceRangeType& _source) { { _dstRange.insert(_dstRange.begin(), _source.begin(), _source.end()) }; }
{
	_dstRange.insert(_dstRange.begin(), _source.begin(), _source.end());
}

template<CEraseableRange _rangeType, typename _valueType>
constexpr inline auto EraseFirst(_rangeType& _range, const _valueType& _value) noexcept
{
	for (auto it = _range.begin(); it != _range.end(); ++it)
	{
		if (*it == _value)
		{
			_range.erase(it);
			return;
		}
	}
}

template<CEraseableRange _rangeType, typename _predType>
constexpr inline auto EraseFirstIf(_rangeType& _range, _predType _pred) noexcept
{
	for (auto it = _range.begin(); it != _range.end(); ++it)
	{
		if (_pred(*it))
		{
			_range.erase(it);
			return;
		}
	}
}

template<CEraseableRange _rangeType, typename _valueType>
constexpr inline auto Erase(_rangeType& _range, const _valueType& _value) noexcept
{
	for (auto it = _range.begin(); it != _range.end(); void())
	{
		if (*it == _value)
			it = _range.erase(it);
		else
			++it;
	}
}

template<CEraseableRange _rangeType, typename _predType>
constexpr inline auto EraseIf(_rangeType& _range, _predType _pred) noexcept
{
	for (auto it = _range.begin(); it != _range.end(); void())
	{
		if (_pred(*it))
			it = _range.erase(it);
		else
			++it;
	}
}

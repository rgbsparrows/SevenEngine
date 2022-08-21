#pragma once

#include "Core/Math/Type.h"
#include "Core/ProgramConfiguation/ConditionDeprecated.h"

#include <type_traits>
#include <concepts>

namespace Math
{
	template<std::unsigned_integral _underlyingType> constexpr inline auto CalcBlockCount(_underlyingType _elementCount, _underlyingType _elementPerBlock) noexcept
	{
		return (_elementCount + _elementPerBlock - 1) / _elementPerBlock;
	}

	template<std::unsigned_integral _underlyingType> constexpr inline auto Calc2PowerAlignedPos(_underlyingType _rawPos, _underlyingType _aligned) noexcept
	{
		return (_rawPos + _aligned - 1) & ~_aligned;
	}

	template<std::unsigned_integral _underlyingType> constexpr inline auto CalcAlignedPos(_underlyingType _rawPos, _underlyingType _aligned) noexcept
	{
		return _rawPos + _aligned - 1 - (_rawPos + _aligned - 1) % _aligned;
	}

	template<typename _underlyingType, size_t _dimension>
	constexpr inline auto operator += (TTypeArray<_underlyingType, _dimension>& _left, TTypeArray<_underlyingType, _dimension> _right)
	{
		for (size_t i = 0; i != _dimension; ++i)
			_left[i] += _right[i];

		return _left;
	}

	template<typename _underlyingType, size_t _dimension>
	constexpr inline auto operator -= (TTypeArray<_underlyingType, _dimension>& _left, TTypeArray<_underlyingType, _dimension> _right)
	{
		for (size_t i = 0; i != _dimension; ++i)
			_left[i] -= _right[i];

		return _left;
	}
}

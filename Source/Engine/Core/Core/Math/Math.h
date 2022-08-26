#pragma once

#include "Core/Math/Type.h"
#include "Core/ProgramConfiguation/ConditionDeprecated.h"

#include <type_traits>
#include <concepts>

namespace Math
{
	constexpr float PI = 3.1415926f;

	inline float Cos(float _angel) noexcept
	{
		return ::cosf(_angel);
	}

	inline double Cos(double _angle) noexcept
	{
		return ::cosl(_angle);
	}

	inline float Sin(float _angle) noexcept
	{
		return ::sinf(_angle);
	}

	inline double Sin(double _angle) noexcept
	{
		return ::sinl(_angle);
	}

	inline float Cot(float _angle) noexcept
	{
		return 1.f / ::tanf(_angle);
	}

	inline double Cot(double _angle) noexcept
	{
		return 1.0 / ::tanl(_angle);
	}

	inline float Tan(float _angle) noexcept
	{
		return ::tanf(_angle);
	}

	inline double Tan(double _angle) noexcept
	{
		return ::tanl(_angle);
	}

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
}

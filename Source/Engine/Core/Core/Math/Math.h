#pragma once

#include "Core/Math/Type.h"
#include "Core/ProgramConfiguation/ConditionDeprecated.h"

#include <type_traits>
#include <concepts>

namespace Math
{
	static constexpr float PI = 3.141593f;
	static constexpr float E = 2.7182818f;
	static constexpr float Log2E = 1.442695f;
	static constexpr float Log10E = 0.434294f;
	static constexpr float Ln2E = 0.693147f;
	static constexpr float Ln10E = 2.302585f;
	static constexpr float Sqrt2 = 1.414214f;

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

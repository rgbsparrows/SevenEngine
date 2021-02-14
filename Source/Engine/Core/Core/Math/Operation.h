#pragma once

#include "Core/Math/Type.h"
#include "Core/Macros/ConditionDeprecated.h"

#include <type_traits>

namespace Math
{
	DEPRECATED_WHEN_CONCEPT_AVAILABLE("use concept std::unsigned_integral in concepts")
	template<typename _underlyingType>
	concept UnsignedInteger = std::is_unsigned_v<_underlyingType>;

	template<UnsignedInteger _underlyingType> constexpr inline auto CalcBlockCount(_underlyingType _elementCount, _underlyingType _elementPerBlock) noexcept
	{
		return (_elementCount + _elementPerBlock - 1) / _elementPerBlock;
	}

	template<UnsignedInteger _underlyingType> constexpr inline auto Calc2PowerAlignedPos(_underlyingType _rawPos, _underlyingType _aligned) noexcept
	{
		return (_rawPos + _aligned - 1) & ~_aligned;
	}

	template<UnsignedInteger _underlyingType> constexpr inline auto CalcAlignedPos(_underlyingType _rawPos, _underlyingType _aligned) noexcept
	{
		return _rawPos + _aligned - 1 - (_rawPos + _aligned - 1) % _aligned;
	}
}

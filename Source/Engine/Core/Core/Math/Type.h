#pragma once

#include <stdint.h>

namespace Math
{
#pragma region type array

	template<typename _underlyingType, size_t _dimension>
	struct TTypeArrayBase
	{
		using UnderlyingType = _underlyingType;
		static constexpr size_t Dimension = _dimension;

		constexpr TTypeArrayBase() noexcept = default;

		constexpr TTypeArrayBase(const UnderlyingType* _data) noexcept
		{
			for (size_t i = 0; i != Dimension; ++i)
				mValue[i] = _data[i];
		}

		constexpr TTypeArrayBase(const TTypeArrayBase&) noexcept = default;

		constexpr UnderlyingType operator[](size_t _index) const noexcept { return mValue[_index]; }
		constexpr UnderlyingType& operator[](size_t _index) noexcept { return mValue[_index]; }

		constexpr UnderlyingType* GetData() noexcept { return mValue; }
		constexpr const UnderlyingType* GetData()const noexcept { return mValue; }

		UnderlyingType mValue[Dimension] = {};
	};

	template<typename _underlyingType, size_t _dimension>
	struct TTypeArray : TTypeArrayBase<_underlyingType, _dimension>
	{
		using UnderlyingType = _underlyingType;
		static constexpr size_t Dimension = _dimension;

		constexpr TTypeArray() noexcept = default;
		constexpr TTypeArray(const TTypeArray&) noexcept = default;

		constexpr TTypeArray(const UnderlyingType* _data) noexcept
			: TTypeArrayBase(_data)
		{
		}
	};

	template<typename _underlyingType>
	struct TTypeArray<_underlyingType, 1> : TTypeArrayBase<_underlyingType, 1>
	{
		using UnderlyingType = _underlyingType;
		static constexpr size_t Dimension = 1;

		constexpr TTypeArray() noexcept = default;
		constexpr TTypeArray(const TTypeArray&) noexcept = default;

		constexpr TTypeArray(const UnderlyingType* _data) noexcept
			: TTypeArrayBase(_data)
		{
		}

		constexpr TTypeArray(UnderlyingType _v0) noexcept
		{
			mValue[0] = _v0;
		}
	};

	template<typename _underlyingType>
	struct TTypeArray<_underlyingType, 2> : TTypeArrayBase<_underlyingType, 2>
	{
		using UnderlyingType = _underlyingType;
		static constexpr size_t Dimension = 1;

		constexpr TTypeArray() noexcept = default;
		constexpr TTypeArray(const TTypeArray&) noexcept = default;

		constexpr TTypeArray(const UnderlyingType* _data) noexcept
			: TTypeArrayBase(_data)
		{
		}

		constexpr TTypeArray(UnderlyingType _v0, UnderlyingType _v1) noexcept
		{
			mValue[0] = _v0;
			mValue[1] = _v1;
		}
	};

	template<typename _underlyingType>
	struct TTypeArray<_underlyingType, 3> : TTypeArrayBase<_underlyingType, 3>
	{
		using UnderlyingType = _underlyingType;
		static constexpr size_t Dimension = 3;

		constexpr TTypeArray() noexcept = default;
		constexpr TTypeArray(const TTypeArray&) noexcept = default;

		constexpr TTypeArray(const UnderlyingType* _data) noexcept
			: TTypeArrayBase(_data)
		{
		}

		constexpr TTypeArray(UnderlyingType _v0, UnderlyingType _v1, UnderlyingType _v2) noexcept
		{
			mValue[0] = _v0;
			mValue[1] = _v1;
			mValue[2] = _v2;
		}
	};

	template<typename _underlyingType>
	struct TTypeArray<_underlyingType, 4> : TTypeArrayBase<_underlyingType, 4>
	{
		using UnderlyingType = _underlyingType;
		static constexpr size_t Dimension = 4;

		constexpr TTypeArray() noexcept = default;
		constexpr TTypeArray(const TTypeArray&) noexcept = default;

		constexpr TTypeArray(const UnderlyingType* _data) noexcept
			: TTypeArrayBase(_data)
		{
		}

		constexpr TTypeArray(UnderlyingType _v0, UnderlyingType _v1, UnderlyingType _v2, UnderlyingType _v3) noexcept
		{
			mValue[0] = _v0;
			mValue[1] = _v1;
			mValue[2] = _v2;
			mValue[3] = _v3;
		}
	};

#pragma endregion

#pragma region rect

	template<typename _underlyingType>
	struct TRect
	{
		using UnderlyingType = _underlyingType;

		constexpr TRect() noexcept = default;
		constexpr TRect(const TRect&) noexcept = default;

		constexpr TRect(UnderlyingType _left, UnderlyingType _on, UnderlyingType _right, UnderlyingType _down) noexcept
			:mLeft(_left), mOn(_on), mRight(_right), mDown(_down)
		{
		}

		constexpr UnderlyingType GetWidth()const noexcept { return mRight - mLeft; }
		constexpr UnderlyingType GetHeight()const noexcept { return mDown - mOn; }

		UnderlyingType mLeft	= UnderlyingType();
		UnderlyingType mOn		= UnderlyingType();
		UnderlyingType mRight	= UnderlyingType();
		UnderlyingType mDown	= UnderlyingType();
	};

#pragma endregion

#pragma region type declear

	using SInt1 = TTypeArray<int32_t, 1>;
	using SInt2 = TTypeArray<int32_t, 2>;
	using SInt3 = TTypeArray<int32_t, 3>;
	using SInt4 = TTypeArray<int32_t, 4>;

	using SUInt1 = TTypeArray<uint32_t, 1>;
	using SUInt2 = TTypeArray<uint32_t, 2>;
	using SUInt3 = TTypeArray<uint32_t, 3>;
	using SUInt4 = TTypeArray<uint32_t, 4>;

	using SFloat1 = TTypeArray<float, 1>;
	using SFloat2 = TTypeArray<float, 2>;
	using SFloat3 = TTypeArray<float, 3>;
	using SFloat4 = TTypeArray<float, 4>;

	using SIntRect = TRect<int32_t>;
	using SUIntRect = TRect<uint32_t>;
	using SFloatRect = TRect<float>;

#pragma endregion
}

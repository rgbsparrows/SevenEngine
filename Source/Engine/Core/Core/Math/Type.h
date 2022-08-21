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
		using BaseType = TTypeArrayBase<_underlyingType, 1>;
		static constexpr size_t Dimension = 1;

		constexpr TTypeArray() noexcept = default;
		constexpr TTypeArray(const TTypeArray&) noexcept = default;

		constexpr TTypeArray(const UnderlyingType* _data) noexcept
			: TTypeArrayBase(_data)
		{
		}

		constexpr TTypeArray(UnderlyingType _v0) noexcept
		{
			BaseType::mValue[0] = _v0;
		}
	};

	template<typename _underlyingType>
	struct TTypeArray<_underlyingType, 2> : TTypeArrayBase<_underlyingType, 2>
	{
		using UnderlyingType = _underlyingType;
		using BaseType = TTypeArrayBase<_underlyingType, 2>;
		static constexpr size_t Dimension = 1;

		constexpr TTypeArray() noexcept = default;
		constexpr TTypeArray(const TTypeArray&) noexcept = default;

		constexpr TTypeArray(const UnderlyingType* _data) noexcept
			: TTypeArrayBase(_data)
		{
		}

		constexpr TTypeArray(UnderlyingType _v0, UnderlyingType _v1) noexcept
		{
			BaseType::mValue[0] = _v0;
			BaseType::mValue[1] = _v1;
		}
	};

	template<typename _underlyingType>
	struct TTypeArray<_underlyingType, 3> : TTypeArrayBase<_underlyingType, 3>
	{
		using UnderlyingType = _underlyingType;
		using BaseType = TTypeArrayBase<_underlyingType, 3>;
		static constexpr size_t Dimension = 3;

		constexpr TTypeArray() noexcept = default;
		constexpr TTypeArray(const TTypeArray&) noexcept = default;

		constexpr TTypeArray(const UnderlyingType* _data) noexcept
			: TTypeArrayBase(_data)
		{
		}

		constexpr TTypeArray(UnderlyingType _v0, UnderlyingType _v1, UnderlyingType _v2) noexcept
		{
			BaseType::mValue[0] = _v0;
			BaseType::mValue[1] = _v1;
			BaseType::mValue[2] = _v2;
		}
	};

	template<typename _underlyingType>
	struct TTypeArray<_underlyingType, 4> : TTypeArrayBase<_underlyingType, 4>
	{
		using UnderlyingType = _underlyingType;
		using BaseType = TTypeArrayBase<_underlyingType, 4>;
		static constexpr size_t Dimension = 4;

		constexpr TTypeArray() noexcept = default;
		constexpr TTypeArray(const TTypeArray&) noexcept = default;

		constexpr TTypeArray(const UnderlyingType* _data) noexcept
			: TTypeArrayBase(_data)
		{
		}

		constexpr TTypeArray(UnderlyingType _v0, UnderlyingType _v1, UnderlyingType _v2, UnderlyingType _v3) noexcept
		{
			BaseType::mValue[0] = _v0;
			BaseType::mValue[1] = _v1;
			BaseType::mValue[2] = _v2;
			BaseType::mValue[3] = _v3;
		}
	};

#pragma endregion

#pragma region matrix

	template<typename _underlyingType, size_t _row, size_t _col>
	struct TTypeMatrix
	{
		static constexpr size_t RowCount = _row;
		static constexpr size_t ColCount = _row;

		using UnderlyingType = _underlyingType;
		using UnderlyingRowType = UnderlyingType[ColCount];
		using UnderlyingMatrixType = UnderlyingType[RowCount][ColCount];

		constexpr TTypeMatrix() noexcept = default;

		constexpr TTypeMatrix(const UnderlyingType* _data) noexcept
		{
			for (size_t i = 0; i != RowCount * ColCount; ++i)
				GetData()[i] = _data[i];
		}

		constexpr TTypeMatrix(const UnderlyingRowType* _data) noexcept
		{
			for (size_t i = 0; i != RowCount; ++i)
				for (size_t j = 0; j != ColCount; ++j)
					mValue[i][j] = _data[i][j];
		}

		constexpr TTypeMatrix(const TTypeMatrix&) noexcept = default;

		constexpr const UnderlyingRowType& operator[](size_t _rowIndex) const noexcept { return mValue[_rowIndex]; }
		constexpr UnderlyingRowType& operator[](size_t _rowIndex) noexcept { return mValue[_rowIndex]; }

		constexpr UnderlyingType* GetData() noexcept { return &mValue[0][0]; }
		constexpr const UnderlyingType* GetData()const noexcept { return &mValue[0][0]; }

		UnderlyingType mValue[RowCount][ColCount] = {};
	};

	template<typename _underlyingType, size_t _row, size_t _col>
	using TTypeMatrixRaw = TTypeMatrix<_underlyingType, _row, _col>::UnderlyingMatrixType;

#pragma region fraction

	template<typename _underlyingType>
	struct TFraction
	{
		using UnderlyingType = _underlyingType;

		constexpr TFraction() noexcept = default;
		constexpr TFraction(const TFraction&) noexcept = default;

		constexpr TFraction(UnderlyingType _numerator, UnderlyingType _denominator) noexcept
			: mNumerator(_numerator), mDenominator(_denominator)
		{
		}

		UnderlyingType mNumerator = UnderlyingType();
		UnderlyingType mDenominator = UnderlyingType();
	};

#pragma endregion

#pragma region rect

	template<typename _underlyingType>
	struct TRect
	{
		using UnderlyingType = _underlyingType;

		constexpr TRect() noexcept = default;
		constexpr TRect(const TRect&) noexcept = default;


		constexpr TRect(TTypeArray<UnderlyingType, 2> _leftOn, TTypeArray<UnderlyingType, 2> _rightDown) noexcept
			:mLeftOn(_leftOn), mRightDwon(_rightDown)
		{
		}

		constexpr TRect(UnderlyingType _left, UnderlyingType _on, UnderlyingType _right, UnderlyingType _down) noexcept
			:mLeftOn(_left, _on), mRightDwon(_right, _down)
		{
		}

		constexpr UnderlyingType GetWidth()const noexcept { return mRightDwon[0] - mLeftOn[0]; }
		constexpr UnderlyingType GetHeight()const noexcept { return mRightDwon[1] - mLeftOn[1]; }

		TTypeArray<UnderlyingType, 2> mLeftOn;
		TTypeArray<UnderlyingType, 2> mRightDwon;
	};

#pragma endregion

#pragma region

	template<typename _underlyingType>
	struct TBox
	{
		using UnderlyingType = _underlyingType;

		constexpr TBox() noexcept = default;
		constexpr TBox(const TBox&) noexcept = default;

		constexpr TBox(TTypeArray<UnderlyingType, 3> _leftOnFront, TTypeArray<UnderlyingType, 3> _rightDownBack) noexcept
			:mLeftOnFront(_leftOnFront), mRightDwonBack(_rightDownBack)
		{
		}

		constexpr TBox(UnderlyingType _left, UnderlyingType _on, UnderlyingType _front, UnderlyingType _right, UnderlyingType _down, UnderlyingType _back) noexcept
			:mLeftOnFront(_left, _on, _front), mRightDwonBack(_right, _down, _back)
		{
		}

		constexpr UnderlyingType GetWidth()const noexcept { return mRightDwonBack[0] - mLeftOnFront[0]; }
		constexpr UnderlyingType GetHeight()const noexcept { return mRightDwonBack[1] - mLeftOnFront[1]; }
		constexpr UnderlyingType GetDepth()const noexcept { return mRightDwonBack[2] - mLeftOnFront[2]; }

		TTypeArray<UnderlyingType, 3> mLeftOnFront;
		TTypeArray<UnderlyingType, 3> mRightDwonBack;
	};

#pragma endregion

#pragma region color

	template<typename _underlyingType>
	struct TColor
	{
		using UnderlyingType = _underlyingType;

		TColor(UnderlyingType _r, UnderlyingType _g, UnderlyingType _b, UnderlyingType _a)
		{
			r = _r;
			g = _g;
			b = _b;
			a = _a;
		}

		TColor()
			:TColor(0, 0, 0, 1)
		{
		}

		TColor(const UnderlyingType* _data)
			:TColor(_data[0], _data[1], _data[2], _data[3])
		{
		}

		constexpr UnderlyingType operator[](size_t _index) const noexcept { return Color[_index]; }
		constexpr UnderlyingType& operator[](size_t _index) noexcept { return Color[_index]; }

		bool operator ==(const TColor& _other) const& noexcept
		{
			return r == _other.r && g == _other.g && b == _other.b && a == _other.a;
		}

		bool operator !=(const TColor&) const& noexcept = default;

		union
		{
			UnderlyingType Color[4];
			struct { UnderlyingType x, y, z, w; };
			struct { UnderlyingType X, Y, Z, W; };
			struct { UnderlyingType r, g, b, a; };
			struct { UnderlyingType R, G, B, A; };
		};
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

	using SFloat4x4 = TTypeMatrix<float, 4, 4>;
	using SFloat4x4Raw = TTypeMatrixRaw<float, 4, 4>;

	using SIntFraction = TFraction<int32_t>;
	using SUIntFraction = TFraction<uint32_t>;

	using SIntRect = TRect<int32_t>;
	using SUIntRect = TRect<uint32_t>;
	using SFloatRect = TRect<float>;

	using SFloatBox = TBox<float>;

	using SFColor = TColor<float>;
	using SUColor32 = uint32_t;

#pragma endregion
}

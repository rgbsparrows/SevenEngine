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

		template<typename _underlyingType2, size_t _dimension2>
		explicit constexpr TTypeArrayBase(const TTypeArrayBase<_underlyingType2, _dimension2>& _other) noexcept
		{
			for (size_t i = 0; i < _dimension2 && i < Dimension; ++i)
				mValue[i] = static_cast<UnderlyingType>(_other[i]);

			for (size_t i = _dimension2; i < Dimension; ++i)
				mValue[i] = UnderlyingType();
		}

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
		using BaseType = TTypeArrayBase<_underlyingType, _dimension>;
		static constexpr size_t Dimension = _dimension;

		constexpr TTypeArray() noexcept = default;
		constexpr TTypeArray(const TTypeArray&) noexcept = default;

		constexpr TTypeArray(const UnderlyingType* _data) noexcept
			: BaseType(_data)
		{
		}

		template<typename _underlyingType2, size_t _dimension2>
		explicit constexpr TTypeArray(const TTypeArray<_underlyingType2, _dimension2>& _other) noexcept
			: BaseType(_other)
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
			: BaseType(_data)
		{
		}

		template<typename _underlyingType2, size_t _dimension2>
		explicit constexpr TTypeArray(const TTypeArray<_underlyingType2, _dimension2>& _other) noexcept
			: BaseType(_other)
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
			: BaseType(_data)
		{
		}

		template<typename _underlyingType2, size_t _dimension2>
		explicit constexpr TTypeArray(const TTypeArray<_underlyingType2, _dimension2>& _other) noexcept
			: BaseType(_other)
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
			: BaseType(_data)
		{
		}

		template<typename _underlyingType2, size_t _dimension2>
		explicit constexpr TTypeArray(const TTypeArray<_underlyingType2, _dimension2>& _other) noexcept
			: BaseType(_other)
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
			: BaseType(_data)
		{
		}

		template<typename _underlyingType2, size_t _dimension2>
		explicit constexpr TTypeArray(const TTypeArray<_underlyingType2, _dimension2>& _other) noexcept
			: BaseType(_other)
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
		static constexpr size_t ColCount = _col;

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

		template<typename _underlyingType2, size_t _row2, size_t _col2>
		explicit operator TTypeMatrix<_underlyingType2, _row2, _col2>() noexcept
		{
			TTypeMatrix<_underlyingType2, _row2, _col2> matrix;

			for (size_t i = 0; i < RowCount && i < _row2; ++i)
				for (size_t j = 0; j < ColCount && j < _col2; ++j)
					matrix[i][j] = (*this)[i][j];

			for (size_t i = _row2; i < RowCount; ++i)
				for (size_t j = _col2; j < ColCount; ++j)
					matrix[i][j] = UnderlyingType();

			return matrix;
		}

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

		TColor()
			:TColor(0, 0, 0, 1)
		{
		}

		TColor(const UnderlyingType* _data)
			:TColor(_data[0], _data[1], _data[2], _data[3])
		{
		}

		TColor(UnderlyingType _r, UnderlyingType _g, UnderlyingType _b, UnderlyingType _a)
		{
			r = _r;
			g = _g;
			b = _b;
			a = _a;
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

		static constexpr TColor Black() noexcept { return TColor(0, 0, 0, 1); }
		static constexpr TColor White() noexcept { return TColor(1, 1, 1, 1); }
		static constexpr TColor Red() noexcept { return TColor(1, 0, 0, 1); }
		static constexpr TColor Green() noexcept { return TColor(0, 1, 0, 1); }
		static constexpr TColor Blue() noexcept { return TColor(0, 0, 1, 1); }
		static constexpr TColor Yellow() noexcept { return TColor(1, 1, 0, 1); }
		static constexpr TColor Cyan() noexcept { return TColor(0, 1, 1, 1); }
		static constexpr TColor Magenta() noexcept { return TColor(1, 0, 1, 1); }
		static constexpr TColor Transparent() noexcept { return TColor(0, 0, 0, 0); }
		static constexpr TColor Gray() noexcept { return TColor(0.5, 0.5, 0.5, 1); }
		static constexpr TColor LightGray() noexcept { return TColor(0.75, 0.75, 0.75, 1); }
		static constexpr TColor DarkGray() noexcept { return TColor(0.25, 0.25, 0.25, 1); }
		static constexpr TColor Orange() noexcept { return TColor(1, 0.5, 0, 1); }
		static constexpr TColor Brown() noexcept { return TColor(0.6, 0.4, 0.2, 1); }
		static constexpr TColor Pink() noexcept { return TColor(1, 0.5, 0.5, 1); }
		static constexpr TColor Clear() noexcept { return TColor(0, 0, 0, 0); }
		static constexpr TColor Purple() noexcept { return TColor(0.5, 0, 0.5, 1); }
		static constexpr TColor Turquoise() noexcept { return TColor(0, 1, 1, 1); }
		static constexpr TColor SkyBlue() noexcept { return TColor(0.5, 0.5, 1, 1); }
		static constexpr TColor Lime() noexcept { return TColor(0, 1, 0, 1); }
		static constexpr TColor ForestGreen() noexcept { return TColor(0.13, 0.55, 0.13, 1); }
		static constexpr TColor Mint() noexcept { return TColor(0.67, 1, 0.67, 1); }
		static constexpr TColor Olive() noexcept { return TColor(0.5, 0.5, 0, 1); }
		static constexpr TColor Maroon() noexcept { return TColor(0.5, 0, 0, 1); }
		static constexpr TColor Navy() noexcept { return TColor(0, 0, 0.5, 1); }
		static constexpr TColor Teal() noexcept { return TColor(0, 0.5, 0.5, 1); }
		static constexpr TColor Silver() noexcept { return TColor(0.75, 0.75, 0.75, 1); }
		static constexpr TColor LimeGreen() noexcept { return TColor(0.2, 0.8, 0.2, 1); }
		static constexpr TColor Indigo() noexcept { return TColor(0.29, 0, 0.51, 1); }
		static constexpr TColor DarkRed() noexcept { return TColor(0.5, 0, 0, 1); }
		static constexpr TColor DarkGreen() noexcept { return TColor(0, 0.5, 0, 1); }
		static constexpr TColor DarkBlue() noexcept { return TColor(0, 0, 0.5, 1); }
		static constexpr TColor YellowGreen() noexcept { return TColor(0.6, 0.8, 0.2, 1); }
		static constexpr TColor PurpleBlue() noexcept { return TColor(0.5, 0, 0.5, 1); }
		static constexpr TColor DarkCyan() noexcept { return TColor(0, 0.5, 0.5, 1); }
		static constexpr TColor Coral() noexcept { return TColor(1, 0.5, 0.31, 1); }
		static constexpr TColor CoralRed() noexcept { return TColor(1, 0.25, 0.25, 1); }
		static constexpr TColor Gold() noexcept { return TColor(1, 0.84, 0, 1); }
		static constexpr TColor LightYellow() noexcept { return TColor(1, 1, 0.88, 1); }
		static constexpr TColor Khaki() noexcept { return TColor(0.94, 0.9, 0.55, 1); }
		static constexpr TColor OrangeRed() noexcept { return TColor(1, 0.27, 0, 1); }
		static constexpr TColor Orchid() noexcept { return TColor(0.85, 0.44, 0.84, 1); }
		static constexpr TColor Salmon() noexcept { return TColor(0.98, 0.5, 0.45, 1); }
		static constexpr TColor Tan() noexcept { return TColor(0.82, 0.71, 0.55, 1); }
		static constexpr TColor Thistle() noexcept { return TColor(0.85, 0.75, 0.85, 1); }
		static constexpr TColor TurquoiseBlue() noexcept { return TColor(0, 0.78, 0.55, 1); }
		static constexpr TColor Violet() noexcept { return TColor(0.93, 0.51, 0.93, 1); }
		static constexpr TColor Wheat() noexcept { return TColor(0.96, 0.87, 0.7, 1); }
	};

#pragma endregion

#pragma region transform

	struct STransform
	{
		TTypeArray<double, 3> mPosition;
		TTypeArray<float, 3> mRotation;
		TTypeArray<float, 3> mScale = TTypeArray<float, 3>(1.f, 1.f, 1.f);
	};

#pragma endregion

#pragma region type declear

	using SUShort1 = TTypeArray<uint16_t, 1>;
	using SUShort2 = TTypeArray<uint16_t, 2>;
	using SUShort3 = TTypeArray<uint16_t, 3>;
	using SUShort4 = TTypeArray<uint16_t, 4>;

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

	using SDouble1 = TTypeArray<double, 1>;
	using SDouble2 = TTypeArray<double, 2>;
	using SDouble3 = TTypeArray<double, 3>;
	using SDouble4 = TTypeArray<double, 4>;

	using SFloat3x3 = TTypeMatrix<float, 3, 3>;
	using SFloat3x3Raw = TTypeMatrixRaw<float, 3, 3>;
	using SFloat3x4 = TTypeMatrix<float, 3, 4>;
	using SFloat3x4Raw = TTypeMatrixRaw<float, 3, 4>;
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

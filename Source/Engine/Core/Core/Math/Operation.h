#pragma once

#include "Core/Math/Type.h"

namespace Math
{
	template<typename _underlyingType, size_t _dimension>
	constexpr inline _underlyingType Dot(const TTypeArray<_underlyingType, _dimension>& _left, const TTypeArray<_underlyingType, _dimension>& _right) noexcept
	{
		_underlyingType res = _underlyingType();

		for (size_t i = 0; i != _dimension; ++i)
			res += _left[i] * _right[i];

		return res;
	}

	template<typename _underlyingType, size_t _dimension>
	constexpr inline auto operator += (TTypeArray<_underlyingType, _dimension>& _left, _underlyingType _right) noexcept
	{
		for (size_t i = 0; i != _dimension; ++i)
			_left[i] += _right;

		return _left;
	}

	template<typename _underlyingType, size_t _dimension>
	constexpr inline auto operator + (const TTypeArray<_underlyingType, _dimension>& _left, _underlyingType _right) noexcept
	{
		TTypeArray<_underlyingType, _dimension> value = _left;
		value += _right;
		return value;
	}

	template<typename _underlyingType, size_t _dimension>
	constexpr inline auto operator += (TTypeArray<_underlyingType, _dimension>& _left, const TTypeArray<_underlyingType, _dimension>& _right) noexcept
	{
		for (size_t i = 0; i != _dimension; ++i)
			_left[i] += _right[i];

		return _left;
	}

	template<typename _underlyingType, size_t _dimension>
	constexpr inline auto operator + (const TTypeArray<_underlyingType, _dimension>& _left, const TTypeArray<_underlyingType, _dimension>& _right) noexcept
	{
		TTypeArray<_underlyingType, _dimension> value = _left;
		value += _right;
		return value;
	}
	template<typename _underlyingType, size_t _dimension>
	constexpr inline auto operator -= (TTypeArray<_underlyingType, _dimension>& _left, _underlyingType _right) noexcept
	{
		for (size_t i = 0; i != _dimension; ++i)
			_left[i] -= _right;

		return _left;
	}

	template<typename _underlyingType, size_t _dimension>
	constexpr inline auto operator - (const TTypeArray<_underlyingType, _dimension>& _left, _underlyingType _right) noexcept
	{
		TTypeArray<_underlyingType, _dimension> value = _left;
		value -= _right;
		return value;
	}

	template<typename _underlyingType, size_t _dimension>
	constexpr inline auto operator -= (TTypeArray<_underlyingType, _dimension>& _left, const TTypeArray<_underlyingType, _dimension>& _right) noexcept
	{
		for (size_t i = 0; i != _dimension; ++i)
			_left[i] -= _right[i];

		return _left;
	}

	template<typename _underlyingType, size_t _dimension>
	constexpr inline auto operator - (const TTypeArray<_underlyingType, _dimension>& _left, const TTypeArray<_underlyingType, _dimension>& _right) noexcept
	{
		TTypeArray<_underlyingType, _dimension> value = _left;
		value -= _right;
		return value;
	}
	template<typename _underlyingType, size_t _dimension>
	constexpr inline auto operator *= (TTypeArray<_underlyingType, _dimension>& _left, _underlyingType _right) noexcept
	{
		for (size_t i = 0; i != _dimension; ++i)
			_left[i] *= _right;

		return _left;
	}

	template<typename _underlyingType, size_t _dimension>
	constexpr inline auto operator * (const TTypeArray<_underlyingType, _dimension>& _left, _underlyingType _right) noexcept
	{
		TTypeArray<_underlyingType, _dimension> value = _left;
		value *= _right;
		return value;
	}

	template<typename _underlyingType, size_t _dimension>
	constexpr inline auto operator *= (TTypeArray<_underlyingType, _dimension>& _left, const TTypeArray<_underlyingType, _dimension>& _right) noexcept
	{
		for (size_t i = 0; i != _dimension; ++i)
			_left[i] *= _right[i];

		return _left;
	}

	template<typename _underlyingType, size_t _dimension>
	constexpr inline auto operator * (const TTypeArray<_underlyingType, _dimension>& _left, const TTypeArray<_underlyingType, _dimension>& _right) noexcept
	{
		TTypeArray<_underlyingType, _dimension> value = _left;
		value *= _right;
		return value;
	}
	template<typename _underlyingType, size_t _dimension>
	constexpr inline auto operator /= (TTypeArray<_underlyingType, _dimension>& _left, _underlyingType _right) noexcept
	{
		for (size_t i = 0; i != _dimension; ++i)
			_left[i] /= _right;

		return _left;
	}

	template<typename _underlyingType, size_t _dimension>
	constexpr inline auto operator / (const TTypeArray<_underlyingType, _dimension>& _left, _underlyingType _right) noexcept
	{
		TTypeArray<_underlyingType, _dimension> value = _left;
		value /= _right;
		return value;
	}

	template<typename _underlyingType, size_t _dimension>
	constexpr inline auto operator /= (TTypeArray<_underlyingType, _dimension>& _left, const TTypeArray<_underlyingType, _dimension>& _right) noexcept
	{
		for (size_t i = 0; i != _dimension; ++i)
			_left[i] /= _right[i];

		return _left;
	}

	template<typename _underlyingType, size_t _dimension>
	constexpr inline auto operator / (const TTypeArray<_underlyingType, _dimension>& _left, const TTypeArray<_underlyingType, _dimension>& _right) noexcept
	{
		TTypeArray<_underlyingType, _dimension> value = _left;
		value /= _right;
		return value;
	}

	template<typename _underlyingType, size_t _dimension, size_t _row, size_t _col>
		requires (_dimension == _row)
	constexpr inline auto operator * (const TTypeArray<_underlyingType, _dimension>& _left, const TTypeMatrix<_underlyingType, _row, _col>& _right) noexcept
	{
		TTypeArray<_underlyingType, _col> resArray;

		for (size_t i = 0; i != _col; ++i)
			for (size_t j = 0; j != _dimension; ++j)
				resArray[i] += _left[j] * _right[j][i];

		return resArray;
	}

	template<typename _underlyingType, size_t _row, size_t _col, size_t _dimension>
		requires (_col == _dimension)
	constexpr inline auto operator * (const TTypeMatrix<_underlyingType, _row, _col>& _left, const TTypeArray<_underlyingType, _dimension>& _right) noexcept
	{
		TTypeArray<_underlyingType, _row> resArray;

		for (size_t i = 0; i != _row; ++i)
			for (size_t j = 0; j != _dimension; ++j)
				resArray[i] += _left[j] * _right[i][j];

		return resArray;
	}

	template<typename _underlyingType, size_t _row1, size_t _col1, size_t _row2, size_t _col2>
		requires (_col1 == _row2)
	constexpr inline auto operator * (const TTypeMatrix<_underlyingType, _row1, _col1>& _left, const TTypeMatrix<_underlyingType, _row2, _col2>& _right) noexcept
	{
		TTypeMatrix<_underlyingType, _row1, _col2> matrix;

		for (size_t i = 0; i != _row1; ++i)
		{
			for (size_t j = 0; j != _col2; ++j)
			{
				for (size_t k = 0; k != _col1; ++k)
				{
					matrix[i][j] += _left[i][k] * _right[k][j];
				}
			}
		}

		return matrix;
	}

	template<typename _underlyingType, size_t _row, size_t _col>
	constexpr inline auto TransposeMatrix(const TTypeMatrix<_underlyingType, _row, _col>& _matrix)
	{
		TTypeMatrix<_underlyingType, _col, _row> result;

		for (size_t i = 0; i != _row; ++i)
			for (size_t j = 0; j != _col; ++j)
				result[j][i] = _matrix[i][j];

		return result;
	}
}

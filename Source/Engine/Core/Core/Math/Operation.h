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
				resArray[i] += resArray[j] * _right[j][i];

		return resArray;
	}

	template<typename _underlyingType, size_t _row, size_t _col, size_t _dimension>
		requires (_col == _dimension)
	constexpr inline auto operator * (const TTypeMatrix<_underlyingType, _row, _col>& _left, const TTypeArray<_underlyingType, _dimension>& _right) noexcept
	{
		TTypeArray<_underlyingType, _row> resArray;

		for (size_t i = 0; i != _row; ++i)
			for (size_t j = 0; j != _dimension; ++j)
				resArray[i] += _right[i][j] * resArray[j];

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

	constexpr inline SFloat4x4 CalcTranslationMatrix(const SFloat3& _position) noexcept
	{
		return SFloat4x4(
			SFloat4x4Raw{
				{1,				0,				0,				0},
				{0,				1,				0,				0},
				{0,				0,				1,				0},
				{_position[0], _position[1],	_position[2],	1},
			}
		);
	}

	//旋转正方向为，当相应轴正方向竖直向上时，从上向下看的顺时针方向
	constexpr inline SFloat4x4 CalcRotationMatrixX(float _angel) noexcept
	{
		return SFloat4x4(
			SFloat4x4Raw{
				{1, 0,				0,				0},
				{0, Cos(_angel),	-Sin(_angel),	0},
				{0, Sin(_angel),	Cos(_angel),	0},
				{0,	0,				0,				1},
			}
		);
	}

	constexpr inline SFloat4x4 CalcRotationMatrixY(float _angel) noexcept
	{
		return SFloat4x4(
			SFloat4x4Raw{
				{Cos(_angel),	0,	Sin(_angel),	0},
				{0,				1,	0,				0},
				{-Sin(_angel),	0,	Cos(_angel),	0},
				{0,				0,	0,				1},
			}
		);
	}

	constexpr inline SFloat4x4 CalcRotationMatrixZ(float _angel) noexcept
	{
		return SFloat4x4(
			SFloat4x4Raw{
				{Cos(_angel),	-Sin(_angel),	0, 0},
				{Sin(_angel),	Cos(_angel),	0, 0},
				{0,				0,				1, 0},
				{0,				0,				0, 1},
			}
		);
	}

	constexpr inline SFloat4x4 CalcRotationMatrix(const SFloat3& _rotation) noexcept
	{
		return SFloat4x4(
			SFloat4x4Raw{
				{Cos(_rotation[1]) * Cos(_rotation[2]),																	-Cos(_rotation[1]) * Sin(_rotation[2]),																Sin(_rotation[1]),						0 },
				{Sin(_rotation[0]) * Sin(_rotation[1]) * Cos(_rotation[2]) + Cos(_rotation[0]) * Sin(_rotation[2]),		-Sin(_rotation[0]) * Sin(_rotation[1]) * Sin(_rotation[2]) + Cos(_rotation[0]) * Cos(_rotation[2]),	-Sin(_rotation[0]) * Cos(_rotation[1]), 0 },
				{-Cos(_rotation[0]) * Sin(_rotation[1]) * Cos(_rotation[2]) + Sin(_rotation[0]) * Sin(_rotation[2]),	Cos(_rotation[0]) * Sin(_rotation[1]) * Sin(_rotation[2]) + Sin(_rotation[0]) * Cos(_rotation[2]),	Cos(_rotation[0]) * Cos(_rotation[1]),	0 },
				{0,																										0,																									0,										1 },
			}
		);
	}

	//Y轴正方向为方向前
	constexpr inline SFloat3 CalcForwardDirection(const SFloat3& _rotation) noexcept
	{
		return SFloat3(Sin(_rotation[0]) * Sin(_rotation[1]) * Cos(_rotation[2]) + Cos(_rotation[0]) * Sin(_rotation[2]), -Sin(_rotation[0]) * Sin(_rotation[1]) * Sin(_rotation[2]) + Cos(_rotation[0]) * Cos(_rotation[2]), -Sin(_rotation[0]) * Cos(_rotation[1]));
	}

	//X轴正方向为方向右
	constexpr inline SFloat3 CalcRightDirection(const SFloat3& _rotation) noexcept
	{
		return SFloat3(Cos(_rotation[1]) * Cos(_rotation[2]), -Cos(_rotation[1]) * Sin(_rotation[2]), Sin(_rotation[1]));
	}

	//Z轴正方向为方向上
	constexpr inline SFloat3 CalcUpDirection(const SFloat3& _rotation) noexcept
	{
		return SFloat3(-Cos(_rotation[0]) * Sin(_rotation[1]) * Cos(_rotation[2]) + Sin(_rotation[0]) * Sin(_rotation[2]), Cos(_rotation[0]) * Sin(_rotation[1]) * Sin(_rotation[2]) + Sin(_rotation[0]) * Cos(_rotation[2]), Cos(_rotation[0]) * Cos(_rotation[1]));
	}

	constexpr inline SFloat4x4 CalcScaleMatrix(const SFloat3& _scale) noexcept
	{
		return SFloat4x4(
			SFloat4x4Raw{
				{_scale[0], 0,			0,			1},
				{0,			_scale[1],	0,			1},
				{0,			0,			_scale[2],	1},
				{0,			0,			0,			1},
			}
		);
	}

	constexpr inline SFloat4x4 CalcTransformMatrix(const SFloat3& _position, const SFloat3& _rotation, const SFloat3& _scale) noexcept
	{
		return CalcScaleMatrix(_scale) * CalcRotationMatrix(_rotation) * CalcTranslationMatrix(_position);
	}

	constexpr inline SFloat4x4 CalcTransformViewMatrix(const SFloat3& _position, const SFloat3& _rotation) noexcept
	{
		SFloat3 forward = CalcForwardDirection(_rotation);
		SFloat3 right = CalcForwardDirection(_rotation);
		SFloat3 up = CalcForwardDirection(_rotation);

		return SFloat4x4(
			SFloat4x4Raw{
				{right[0],		forward[0],		up[0],			0},
				{right[1],		forward[1],		up[1],			0},
				{right[2],		forward[2],		up[2],			0},
				{-_position[0],	-_position[1],	-_position[2],	1},
			}
		);
	}

	constexpr inline SFloat4x4 CalcPerspectiveProjectMatrix(float _fov, float _near, float _far, float _width, float _height) noexcept
	{
		return SFloat4x4(
			SFloat4x4Raw{
				{Cot(_fov / 2.f) * _height / _width,	0,						0,								0},
				{0,											Cot(_fov / 2.f),	0,								0},
				{0,											0,						_near / (_near - _far),			1},
				{0,											0,						_near * _far / (_far - _near),	0},
			}
		);
	}

	constexpr inline SFloat4x4 CalcParallelProjectMatrix(float _near, float _far, float _width, float _height) noexcept
	{
		return SFloat4x4(
			SFloat4x4Raw{
				{0.5f / _height,	0,				0,						0},
				{0,					0.5f / _height,	0,						0},
				{0,					0,				1 / (_near - _far),		0},
				{0,					0,				_far / (_far - _near),	1},
			}
		);
	}
}

#pragma once

#include "Core/Math/Type.h"
#include "Core/Math/Operation.h"
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

	inline float ATan2(float _x, float _y) noexcept
	{
		return ::atan2f(_y, _x);
	}

	inline double ATan2(double _x, double _y) noexcept
	{
		return ::atan2l(_y, _x);
	}

	inline float Sqrt(float _v) noexcept
	{
		return ::sqrtf(_v);
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
				{_scale[0], 0,			0,			0},
				{0,			_scale[1],	0,			0},
				{0,			0,			_scale[2],	0},
				{0,			0,			0,			1},
			}
		);
	}

	constexpr inline SFloat4x4 CalcTransformMatrix(const SFloat3& _position, const SFloat3& _rotation, const SFloat3& _scale) noexcept
	{
		return CalcScaleMatrix(_scale) * CalcRotationMatrix(_rotation) * CalcTranslationMatrix(_position);
	}

	constexpr inline SFloat4x4 CalcTransformMatrix(const STransform& _transform) noexcept
	{
		return CalcTransformMatrix(static_cast<SFloat3>(_transform.mPosition), _transform.mRotation, _transform.mScale);
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

	inline STransform ApplyTransform(const STransform& _parentTransform, const STransform& _localTransform) noexcept
	{
		SFloat4x4 parentTransformMatrix = CalcRotationMatrix(_parentTransform.mRotation);
		SFloat4x4 localTransformMatrix = CalcTransformMatrix(static_cast<SFloat3>(_localTransform.mPosition), _localTransform.mRotation, SFloat3(1, 1, 1));

		SFloat4x4 transformMatrix = localTransformMatrix * parentTransformMatrix;

		SFloat3 cosList;
		SFloat3 sinList;

		sinList[1] = transformMatrix[0][2];
		cosList[1] = Sqrt(1 - sinList[1] * sinList[1]);

		if (cosList[1] == 0)
		{
			cosList[0] = 1;
			sinList[0] = 0;
			cosList[2] = transformMatrix[1][2];
			sinList[2] = transformMatrix[1][0];
		}
		else
		{
			cosList[0] = transformMatrix[2][2] / cosList[1];
			sinList[0] = -transformMatrix[1][2] / cosList[1];
			cosList[2] = transformMatrix[0][0] / cosList[1];
			sinList[2] = -transformMatrix[0][1] / cosList[1];
		}

		STransform transform;
		transform.mPosition = _parentTransform.mPosition + Math::SDouble3(transformMatrix[3][0], transformMatrix[3][1], transformMatrix[3][2]);
		transform.mRotation = SFloat3(ATan2(cosList[0], sinList[0]), ATan2(cosList[1], sinList[1]), ATan2(cosList[2], sinList[2]));
		transform.mScale = _parentTransform.mScale * _localTransform.mScale;

		return transform;
	}
}

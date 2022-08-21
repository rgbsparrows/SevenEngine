#pragma once

#include "Core/Math/Type.h"

namespace GameFramework
{
	struct STransform
	{
		Math::SFloat3 mPosition;
		Math::SFloat3 mRotation;
		Math::SFloat3 mScale;

		Math::SFloat4x4 mWorldMatrix;
		Math::SFloat4x4 mLocalMatrix;

		void UpdateTransform(const STransform* _parentTransform);
	};
}

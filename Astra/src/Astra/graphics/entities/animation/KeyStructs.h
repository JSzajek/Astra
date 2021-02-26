#pragma once

#include "Astra/math/Vec3.h"
#include "Astra/math/Quat.h"

namespace Astra::Graphics
{
	struct KeyPosition
	{
		Math::Vec3 Position;
		float timeStamp;

		KeyPosition() = default;
		KeyPosition(float time, const Math::Vec3& position)
			: timeStamp(time), Position(position)
		{
		}
	};

	struct KeyRotation
	{
		Math::Quat Orientation;
		float timeStamp;

		KeyRotation() = default;
		KeyRotation(float time, const Math::Quat& orientation)
			: timeStamp(time), Orientation(orientation)
		{
		}
	};

	struct KeyScale
	{
		Math::Vec3 Scale;
		float timeStamp;
		
		KeyScale() = default;
		KeyScale(float time, const Math::Vec3 &scale)
			: timeStamp(time), Scale(scale)
		{
		}
	};
}
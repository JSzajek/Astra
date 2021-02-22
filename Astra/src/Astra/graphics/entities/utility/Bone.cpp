#include "astra_pch.h"

#include "Bone.h"

#include "Astra/math/Mat4Utils.h"

namespace Astra::Graphics
{
	Bone::Bone(const std::string& name, int id, const KeyFrameInfo& info)
		: m_name(name), m_id(id), m_info(info)
	{
	}

	Math::Mat4 Bone::Update(float time)
	{
		return Math::Mat4(InterpolatePosition(time), InterpolateRotation(time), InterpolateScaling(time));
	}

	const int Bone::GetPositionIndex(float time)
	{
		for (unsigned int i = 0; i < m_info.PositionsCount - 1; ++i)
		{
			if (time < m_info.Positions[i + 1].timeStamp)
			{
				return i;
			}
		}
		ASTRA_CORE_WARN("Bone: Couldn't Find Position Index for Time {0}.", time);
		return 0;
	}

	const int Bone::GetRotationIndex(float time)
	{
		for (unsigned int i = 0; i < m_info.RotationCount - 1; ++i)
		{
			if (time < m_info.Rotations[i + 1].timeStamp)
			{
				return i;
			}
		}
		ASTRA_CORE_WARN("Bone: Couldn't Find Rotation Index for Time {0}.", time);
		return 0;
	}

	const int Bone::GetScaleIndex(float time)
	{
		for (unsigned int i = 0; i < m_info.ScalesCount - 1; ++i)
		{
			if (time < m_info.Scales[i + 1].timeStamp)
			{
				return i;
			}
		}
		ASTRA_CORE_WARN("Bone: Couldn't Find Scale Index for Time {0}.", time);
		return 0;
	}

	float Bone::GetScaleFactor(float lastTime, float nextTime, float time)
	{
		return (time - lastTime) / (nextTime - lastTime);
	}

	Math::Vec3 Bone::InterpolatePosition(float time)
	{
		if (m_info.PositionsCount == 1)
		{
			return m_info.Positions[0].Position;
		}

		auto index = GetPositionIndex(time);
		auto scaleFactor = GetScaleFactor(m_info.Positions[index].timeStamp,
										  m_info.Positions[index + 1].timeStamp, time);
		auto finalPosition = m_info.Positions[index].Position.Mix(m_info.Positions[index + 1].Position, scaleFactor);
		return finalPosition;
	}

	Math::Quat Bone::InterpolateRotation(float time)
	{
		if (m_info.RotationCount == 1)
		{
			return m_info.Rotations[0].Orientation.Normalized();
		}

		auto index = GetRotationIndex(time);
		auto scaleFactor = GetScaleFactor(m_info.Rotations[index].timeStamp,
										  m_info.Rotations[index + 1].timeStamp, time);
		auto first = m_info.Rotations[index].Orientation;
		auto second = m_info.Rotations[index + 1].Orientation;
		auto finalPosition = m_info.Rotations[index].Orientation.Slerp(m_info.Rotations[index + 1].Orientation, scaleFactor);

		return finalPosition;
	}

	Math::Vec3 Bone::InterpolateScaling(float time)
	{
		if (m_info.ScalesCount == 1)
		{
			return m_info.Scales[0].Scale;
		}

		auto index = GetScaleIndex(time);
		auto scaleFactor = GetScaleFactor(m_info.Scales[index].timeStamp,
										  m_info.Scales[index + 1].timeStamp, time);
		auto finalPosition = m_info.Scales[index].Scale.Slerp(m_info.Scales[index + 1].Scale, scaleFactor);
		return finalPosition;
	}
}
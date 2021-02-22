#pragma once

#include <vector>
#include <string>

#include <assimp/anim.h>

#include "Astra/math/Mat4.h"

#include "Astra/graphics/entities/utility/BoneReader.h"

namespace Astra::Graphics
{
	class Bone
	{
	private:
		KeyFrameInfo m_info;

		Math::Mat4 m_localTransform;
		std::string m_name;
		int m_id;
	public:
		Bone() = default;
		Bone(const std::string& name, int id, const KeyFrameInfo& info);

		Math::Mat4 Update(float time);

		inline const std::string& GetName() const { return m_name; }
		inline const int GetId() const { return m_id; }

		const int GetPositionIndex(float time);
		const int GetRotationIndex(float time);
		const int GetScaleIndex(float time);
	private:
		float GetScaleFactor(float lastTime, float nextTime, float time);
		Math::Vec3 InterpolatePosition(float time);
		Math::Quat InterpolateRotation(float time);
		Math::Vec3 InterpolateScaling(float time);
	};
}
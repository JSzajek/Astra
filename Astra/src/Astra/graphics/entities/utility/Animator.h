#pragma once

#include <vector>

#include "Astra/graphics/entities/utility/Animation.h"
#include "Astra/math/Mat4.h"

namespace Astra::Graphics
{
	class Animator
	{
	private:
		std::vector<Math::Mat4> m_finalBoneOffsets;
		Math::Mat4 m_globalInverseTransform;
		Animation* m_current;
		float m_currentTime;
		bool m_playing;
	public:
		Animator() = default;

		inline const std::vector<Math::Mat4>& GetOffsets() const { return m_finalBoneOffsets; }
		inline const unsigned int GetCount() const { return m_finalBoneOffsets.size(); }

		void UpdateAnimation(float delta);
		inline void SetGlobalInverseTransform(const Math::Mat4& mat) {
			m_globalInverseTransform = mat;
		}
		void SetAnimation(Astra::Graphics::Animation* animation);
		inline void Play() { m_playing = true; }
	private:
		void CalculateBoneTransform(const NodeData* node, const Math::Mat4& parentTransform);

	};
}
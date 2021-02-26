#pragma once

#include <vector>
#include <thread>

#include "Astra/graphics/entities/animation/Animation.h"
#include "Astra/math/Mat4.h"

namespace Astra::Graphics
{
	class Animator
	{
	private:
		std::vector<Math::Mat4> m_finalBoneOffsets;
		Asset<Animation> m_current;
		float m_currentTime = 0.0f;
		bool m_playing = false;
	public:
		Animator() = default;
		Animator(const Animator& other);

		inline const std::vector<Math::Mat4>& GetOffsets() const { return m_finalBoneOffsets; }
		inline const unsigned int GetCount() const { return m_finalBoneOffsets.size(); }

		void UpdateAnimation(float delta);
		void SetAnimation(const Asset<Animation>& animation);
		std::string GetNameOfPlayingAnimation() const { return m_current->GetName(); }
		inline void Play() { m_playing = true; }
	private:
		void CalculateBoneTransform(const NodeData* node, const Math::Mat4& parentTransform);
	};
}
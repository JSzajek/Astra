#include "astra_pch.h"

#include "Animator.h"

namespace Astra::Graphics
{
	Animator::Animator(const Animator& other)
		: m_finalBoneOffsets(other.m_finalBoneOffsets), m_current(other.m_current), 
			m_currentTime(other.m_currentTime), m_playing(other.m_playing)
	{
	}

	void Animator::UpdateAnimation(float delta)
	{
		if (m_playing)
		{
			m_currentTime += m_current->GetTicksPerSecond() * delta;
			m_currentTime = fmodf(m_currentTime, m_current->GetDuration());
			CalculateBoneTransform(&m_current->GetRootNode(), Math::Mat4(1));
		}
	}

	void Animator::SetAnimation(const Asset<Animation>& animation)
	{
		m_current = animation;
		m_currentTime = 0.0f;

		auto size = animation->GetBoneInfo().size();
		m_finalBoneOffsets.reserve(size);

		for (unsigned int i = 0; i < size; ++i)
		{
			m_finalBoneOffsets.push_back(Math::Mat4(1));
		}
	}

	void Animator::CalculateBoneTransform(const NodeData* node, const Math::Mat4& parentTransform)
	{
		std::string nodeName = node->Name;
		Bone* bone = m_current->FindBone(nodeName);
		
		Math::Mat4 globalTransform = parentTransform * (bone ? bone->Update(m_currentTime) : node->Transformation);

		auto infoMap = m_current->GetBoneInfo();
		if (infoMap.find(nodeName) != infoMap.end())
		{
			m_finalBoneOffsets[infoMap[nodeName].Id] = globalTransform * infoMap[nodeName].Offset;
		}

		for (unsigned int i = 0; i < node->Count; ++i)
		{
			CalculateBoneTransform(&node->Children[i], globalTransform);
		}
	}
}
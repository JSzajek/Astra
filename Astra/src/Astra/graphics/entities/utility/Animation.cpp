#include "astra_pch.h"

#include "Animation.h"
#include "Astra/graphics/entities/utility/AssimpReader.h"

namespace Astra::Graphics
{
	Animation::Animation(const aiAnimation* animation, const aiNode* root, std::map<std::string, BoneInfo>& boneMap, int& boneCount)
		: m_name(animation->mName.C_Str()), m_duration(static_cast<float>(animation->mDuration)), m_ticksPerSecond(static_cast<float>(animation->mTicksPerSecond))
	{
		ReadData(m_root, root);
		ValidateBones(animation, boneMap, boneCount);
	}

	Bone* Animation::FindBone(const std::string& name)
	{
		auto found = std::find_if(m_bones.begin(), m_bones.end(), [&](const Bone& bone) { return bone.GetName() == name; });
		if (found != m_bones.end())
		{
			return &(*found);
		}
		return NULL;
	}

	void Animation::ValidateBones(const aiAnimation* animation, std::map<std::string, BoneInfo>& boneMap, int& boneCount)
	{
		for (unsigned int i = 0; i < animation->mNumChannels; ++i)
		{
			auto channel = animation->mChannels[i];
			std::string boneName = channel->mNodeName.data;

			if (boneMap.find(boneName) == boneMap.end())
			{
				boneMap[boneName].Id = boneCount;
				boneCount++;
			}
			m_bones.push_back(Bone(boneName, boneMap[boneName].Id, ReadKeyFrames(channel)));
		}
		m_boneInfoMap = boneMap;
	}

	void Animation::ReadData(NodeData& destination, const aiNode* source)
	{
		ASTRA_CORE_ASSERT(source, "Animation: No Source Data Found.");

		destination.Name = source->mName.data;
		destination.Transformation = ConvertAiMatrix(source->mTransformation);
		destination.Count = source->mNumChildren;
		destination.Children.reserve(destination.Count);

		for (unsigned int i = 0; i < source->mNumChildren; ++i)
		{
			NodeData data;
			ReadData(data, source->mChildren[i]);
			destination.Children.push_back(data);
		}
	}
}
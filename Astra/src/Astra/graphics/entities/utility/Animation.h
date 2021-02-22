#pragma once

#include <vector>
#include <map>
#include <string>

#include <assimp/anim.h>
#include <assimp/scene.h>

#include "Astra/graphics/entities/utility/Bone.h"

namespace Astra::Graphics
{
	struct NodeData
	{
		Math::Mat4 Transformation;
		std::string Name;
		std::vector<NodeData> Children;
		unsigned int Count;
	};

	class Animation
	{
	private:
		std::vector<Bone> m_bones;
		std::map<std::string, BoneInfo> m_boneInfoMap;
		NodeData m_root;
		float m_duration;
		float m_ticksPerSecond;
	public:
		Animation() = default;
		Animation(const aiAnimation* animation, const aiNode* root, std::map<std::string, BoneInfo>& boneMap, int& boneCount);

		inline float GetTicksPerSecond() const { return m_ticksPerSecond; }
		inline float GetDuration() const { return m_duration; }
		inline const NodeData& GetRootNode() { return m_root; }
		inline const std::map<std::string, BoneInfo>& GetBoneInfo() { return m_boneInfoMap; }
		inline unsigned int GetBoneCount() { return m_boneInfoMap.size(); }

		Bone* FindBone(const std::string& name);
	private:
		void ValidateBones(const aiAnimation* animation, std::map<std::string, BoneInfo>& boneMap, int& boneCount);
		void ReadData(NodeData& dest, const aiNode* source);
	};
}
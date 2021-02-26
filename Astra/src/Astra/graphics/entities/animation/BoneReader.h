#pragma once

#include <tuple>
#include <vector>

#include <assimp/anim.h>

#include "Astra/graphics/entities/animation/BoneInfo.h"
#include "Astra/graphics/entities/animation/KeyStructs.h"
#include "Astra/graphics/entities/animation/AssimpReader.h"

namespace Astra::Graphics
{
	struct KeyFrameInfo
	{
		std::vector<KeyPosition> Positions;
		std::vector<KeyRotation> Rotations;
		std::vector<KeyScale> Scales;

		unsigned int PositionsCount = 0;
		unsigned int RotationCount = 0;
		unsigned int ScalesCount = 0;
	};

	static KeyFrameInfo ReadKeyFrames(const aiNodeAnim* channel)
	{
		KeyFrameInfo result;

		result.PositionsCount = channel->mNumPositionKeys;
		result.Positions.reserve(result.PositionsCount);
		for (unsigned int i = 0; i < result.PositionsCount; ++i)
		{
			result.Positions.push_back(KeyPosition(static_cast<float>(channel->mPositionKeys[i].mTime), 
												   ConvertAiVector(channel->mPositionKeys[i].mValue)));
		}

		result.RotationCount= channel->mNumRotationKeys;
		result.Rotations.reserve(result.RotationCount);
		for (unsigned int i = 0; i < result.RotationCount; ++i)
		{
			result.Rotations.push_back(KeyRotation(static_cast<float>(channel->mRotationKeys[i].mTime), 
												   ConvertAiQuat(channel->mRotationKeys[i].mValue)));
		}
		
		result.ScalesCount = channel->mNumScalingKeys;
		result.Scales.reserve(result.ScalesCount);
		for (unsigned int i = 0; i < result.ScalesCount; ++i)
		{
			result.Scales.push_back(KeyScale(static_cast<float>(channel->mScalingKeys[i].mTime), 
											 ConvertAiVector(channel->mScalingKeys[i].mValue)));
		}

		return result;
	}
}
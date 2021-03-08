#pragma once

#include "Astra/math/Vec3.h"

namespace Astra::Graphics
{
#define MAX_BONES			75
#define MAX_BONE_INFLUENCE	3

	struct Vertex
	{
		// Base Information
		Math::Vec3 Position;
		Math::Vec2 TextureCoords;
		Math::Vec3 Normal;

		// Animation Information
		int BoneIds[MAX_BONE_INFLUENCE] = { -1, -1, -1 };
		float Weights[MAX_BONE_INFLUENCE] = { 0.0f, 0.0f, 0.0f };

		Vertex() = default;

		Vertex(float xPos, float yPos, float zPos,
			float xNorm, float yNorm, float zNorm)
			: Position(xPos, yPos, zPos), TextureCoords(0), Normal(xNorm, yNorm, zNorm)
		{
		}

		Vertex(float xPos, float yPos, float zPos, float xTex, float yTex,
			float xNorm, float yNorm, float zNorm)
			: Position(xPos, yPos, zPos), TextureCoords(xTex, yTex), Normal(xNorm, yNorm, zNorm)
		{
		}
	};

	struct NormalVertex : public Vertex
	{
		// Normal Mapping Information
		Math::Vec3 Tangent;

		NormalVertex() = default;

		NormalVertex(float xPos, float yPos, float zPos,
			float xNorm, float yNorm, float zNorm,
			float xTang, float yTang, float zTang)
			: Vertex(xPos, yPos, zPos, xNorm, yNorm, zNorm),
			Tangent(xTang, yTang, zTang)
		{
		}

		NormalVertex(float xPos, float yPos, float zPos, float xTex, float yTex,
			float xNorm, float yNorm, float zNorm,
			float xTang, float yTang, float zTang)
			: Vertex(xPos, yPos, zPos, xTex, yTex, xNorm, yNorm, zNorm),
			Tangent(xTang, yTang, zTang)
		{
		}
	};
}
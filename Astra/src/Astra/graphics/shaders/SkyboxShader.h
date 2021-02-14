#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	#define FIRST_CUBE_MAP_TAG		"cubeMap"
	#define SECOND_CUBE_MAP_TAG		"cubeMap2"
	#define BLEND_FACTOR_TAG		"blendFactor"

	class SkyboxShader : public Shader
	{
	public:
		SkyboxShader(const char* filepath =
			"../Astra/res/shaders/Skybox.shader")
			: Shader(filepath)
		{
		}
	};
}
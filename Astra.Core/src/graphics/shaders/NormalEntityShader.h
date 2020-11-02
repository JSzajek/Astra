#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	#define FAKE_LIGHT					"useFakeLighting"
	#define NUMBER_OF_ROWS				"numberOfRows"
	#define OFFSET_TAG					"offset"
	
	#define NORMAL_MAP					"material.normalMap"

	class NormalEntityShader : public Shader
	{
	public:
		NormalEntityShader(int numOfLights, const char* filepath =
			"../Astra.Core/src/resources/shaders/NormalEntity.shader")
			: Shader(filepath, ShaderType::NormalMapped, &std::make_tuple("NR_POINT_LIGHTS %i", numOfLights))
		{
		}
	};
}
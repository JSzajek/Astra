#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	#define FAKE_LIGHT					"useFakeLighting"
	#define NUMBER_OF_ROWS				"numberOfRows"
	#define OFFSET_TAG					"offset"
	
	#define NORMAL_MAP					"material.normalMap"
	#define PARALLAX_MAP				"material.parallaxMap"

	#define NORMAL_MAPPED_FLAG_TAG		"flags[0]"
	#define PARALLAX_MAPPED_FLAG_TAG	"flags[1]"
	
	#define HEIGHT_SCALE				"heightScale"

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
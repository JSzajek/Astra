#pragma once

#include "Shader.h"

namespace Astra::Graphics
{	
	#define FAKE_LIGHT					"useFakeLighting"
	#define NUMBER_OF_ROWS				"numberOfRows"
	#define OFFSET_TAG					"offset"

	#define GLOWING						"glowing"
	#define EMISSION_MAP				"material.emissionMap"

	class EntityShader : public Shader
	{
	public:
		EntityShader(int numOfLights, const char* filepath =
			"../Astra.Core/src/resources/shaders/Entity.shader")
			: Shader(filepath, &std::make_tuple("NR_POINT_LIGHTS %i", numOfLights))
		{
		}
	};
}
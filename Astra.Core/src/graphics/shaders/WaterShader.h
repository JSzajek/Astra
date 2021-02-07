#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	#define NEAR_PLANE					"nearPlane"
	#define FAR_PLANE					"farPlane"

	#define REFLECTION_TEXTURE			"material.reflectionTexture"
	#define REFRACTION_TEXTURE			"material.refractionTexture"
	#define DUDVMAP_TEXTURE				"material.dudvMap"
	#define NORMALMAP_TEXTURE			"material.normalMap"
	#define DEPTHMAP_TEXTURE			"material.depthMap"

	#define BASE_WATER_COLOR			"baseWaterColor"
	#define MOVE_FACTOR					"moveFactor"
	#define WAVE_STRENGTH				"waveStrength"

	class WaterShader : public Shader
	{
	public:
		WaterShader(int numOfLights, const char* filepath =
			"../Astra.Core/src/resources/shaders/Water.shader")
			: Shader(filepath, &std::make_tuple("NR_POINT_LIGHTS %i", numOfLights))
		{
		}
	};
}
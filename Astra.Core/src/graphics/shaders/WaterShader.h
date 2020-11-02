#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	#define CLIP_PLANE					"clipPlane"
	#define NEAR_PLANE					"nearPlane"
	#define FAR_PLANE					"farPlane"

	#define NORMAL_MATRIX				"normalMatrix"

	#define FOG_COLOR					"fogColor"

	#define REFLECTION_TEXTURE			"material.reflectionTexture"
	#define REFRACTION_TEXTURE			"material.refractionTexture"
	#define DUDVMAP_TEXTURE				"material.dudvMap"
	#define NORMALMAP_TEXTURE			"material.normalMap"
	#define DEPTHMAP_TEXTURE			"material.depthMap"
	#define SPECULAR_MAP				"material.specularMap"
	#define MATERIAL_REFLECTIVITY		"material.reflectivity"

	#define BASE_WATER_COLOR			"baseWaterColor"
	#define MOVE_FACTOR					"moveFactor"
	#define WAVE_STRENGTH				"waveStrength"
	#define CAMERA_POSITION				"cameraPosition"

	#define DIR_LIGHT_DIRECTION			"directionalLight.direction"
	#define DIR_LIGHT_AMBIENT			"directionalLight.ambient"
	#define DIR_LIGHT_DIFFUSE			"directionalLight.diffuse"
	#define DIR_LIGHT_SPECULAR			"directionalLight.specular"

	class WaterShader : public Shader
	{
	public:
		WaterShader(int numOfLights, const char* filepath =
			"../Astra.Core/src/resources/shaders/Water.shader")
			: Shader(filepath, ShaderType::Water, &std::make_tuple("NR_POINT_LIGHTS %i", numOfLights))
		{
		}
	};
}
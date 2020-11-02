#pragma once

#include "Shader.h"

namespace Astra::Graphics
{	
	#define FAKE_LIGHT					"useFakeLighting"
	#define NUMBER_OF_ROWS				"numberOfRows"
	#define OFFSET_TAG					"offset"
	#define CLIP_PLANE					"clipPlane"

	#define NORMAL_MATRIX				"normalMatrix"

	#define FOG_COLOR					"fogColor"

	#define DIFFUSE_MAP					"material.diffuseMap"
	#define SPECULAR_MAP				"material.specularMap"
	#define MATERIAL_REFLECTIVITY		"material.reflectivity"

	#define DIR_LIGHT_DIRECTION			"directionalLight.direction"
	#define DIR_LIGHT_AMBIENT			"directionalLight.ambient"
	#define DIR_LIGHT_DIFFUSE			"directionalLight.diffuse"
	#define DIR_LIGHT_SPECULAR			"directionalLight.specular"

	class EntityShader : public Shader
	{
	public:
		EntityShader(int numOfLights, const char* filepath =
			"../Astra.Core/src/resources/shaders/Entity.shader")
			: Shader(filepath, ShaderType::Lighting, &std::make_tuple("NR_POINT_LIGHTS %i", numOfLights))
		{
		}
	};
}
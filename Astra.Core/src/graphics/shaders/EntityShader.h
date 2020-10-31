#pragma once

#include "Shader.h"

namespace Astra::Graphics
{	
	#define NORMAL_MATRIX				"normalMatrix"

	#define DIFFUSE_MAP					"material.diffuseMap"
	#define SPECULAR_MAP				"material.specularMap"
	#define MATERIAL_REFLECTIVITY		"material.reflectivity"

	#define LIGHT_VECTOR				"light.l_vector"
	#define LIGHT_DIRECTION				"light.direction"
	#define LIGHT_AMBIENT				"light.ambient"
	#define LIGHT_DIFFUSE				"light.diffuse"
	#define LIGHT_SPECULAR				"light.specular"
	#define LIGHT_ATTENUATION			"light.attenuation"
	#define LIGHT_CUTOFF				"light.cutOff"
	#define LIGHT_OUTER_CUTOFF			"light.outerCutOff"

	class EntityShader : public Shader
	{
	public:
		static constexpr const char* LightPositionTag = "lightPosition";
		static constexpr const char* LightColorTag = "lightPosition";
	public:
		EntityShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/Entity.shader")
			: Shader(filepath, ShaderType::Lighting)
		{
		}
	};
}
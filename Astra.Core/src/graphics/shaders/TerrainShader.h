#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	#define CLIP_PLANE					"clipPlane"

	#define NORMAL_MATRIX				"normalMatrix"

	#define FOG_COLOR					"fogColor"

	#define BACKGROUND_TEXTURE			"material.backgroundTexture"
	#define R_TEXTURE					"material.rTexture"
	#define G_TEXTURE					"material.gTexture"
	#define B_TEXTURE					"material.bTexture"
	#define BLEND_MAP					"material.blendMap"
	#define SPECULAR_MAP				"material.specularMap"
	#define MATERIAL_REFLECTIVITY		"material.reflectivity"

	#define DIR_LIGHT_DIRECTION			"directionalLight.direction"
	#define DIR_LIGHT_AMBIENT			"directionalLight.ambient"
	#define DIR_LIGHT_DIFFUSE			"directionalLight.diffuse"
	#define DIR_LIGHT_SPECULAR			"directionalLight.specular"

	class TerrainShader : public Shader
	{
	public:
		//static constexpr const char* ClipPaneTag = "clipPlane";
	public:
		TerrainShader(int numOfLights, const char* filepath =
			"../Astra.Core/src/resources/shaders/Terrain.shader")
			: Shader(filepath, ShaderType::Terrains, &std::make_tuple("NR_POINT_LIGHTS %i", numOfLights))
		{
		}
	};
}
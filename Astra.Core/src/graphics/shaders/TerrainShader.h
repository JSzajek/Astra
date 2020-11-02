#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	#define BACKGROUND_TEXTURE		"material.backgroundTexture"
	#define R_TEXTURE				"material.rTexture"
	#define G_TEXTURE				"material.gTexture"
	#define B_TEXTURE				"material.bTexture"
	#define BLEND_MAP				"material.blendMap"

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
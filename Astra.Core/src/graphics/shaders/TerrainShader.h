#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class TerrainShader : public Shader
	{
	public:
		static constexpr const char* LightPositionTag = "lightPosition";
		static constexpr const char* LightColorTag = "lightColor";
		static constexpr const char* ShineDampenerTag = "shineDampener";
		static constexpr const char* ReflectivityTag = "reflectivity";

		static constexpr const char* SkyColorTag = "skyColor";
	public:
		TerrainShader(const char* filepath =
			"C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/shaders/Terrain.shader")
			: Shader(filepath, ShaderType::Terrains)
		{
		}
	};
}
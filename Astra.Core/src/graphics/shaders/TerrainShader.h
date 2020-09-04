#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class TerrainShader : public Shader
	{
	public:
		TerrainShader(const char* filepath =
			"C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/shaders/Terrain.shader")
			: Shader(filepath)
		{
		}
	};
}
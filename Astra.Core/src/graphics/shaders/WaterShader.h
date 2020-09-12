#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class WaterShader : public Shader
	{
	public:
		WaterShader(const char* filepath =
			"C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/shaders/Water.shader")
			: Shader(filepath, ShaderType::Water)
		{
		}
	};
}
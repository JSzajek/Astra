#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class SkyboxShader : public Shader
	{
	public:
		SkyboxShader(const char* filepath =
			"C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/shaders/Skybox.shader")
			: Shader(filepath, ShaderType::Skybox)
		{
		}
	};
}
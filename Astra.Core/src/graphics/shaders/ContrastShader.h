#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class ContrastShader : public Shader
	{
	public:
		ContrastShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/Contrast.shader")
			: Shader(filepath, ShaderType::Basic)
		{
		}
	};
}
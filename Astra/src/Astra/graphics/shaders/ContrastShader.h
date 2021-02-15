#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class ContrastShader : public Shader
	{
	public:
		ContrastShader(const char* filepath =
			RESOURCE("res/shaders/Contrast.shader"))
			: Shader(filepath)
		{
		}
	};
}
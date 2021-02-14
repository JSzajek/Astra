#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class ContrastShader : public Shader
	{
	public:
		ContrastShader(const char* filepath =
			"../Astra/res/shaders/Contrast.shader")
			: Shader(filepath)
		{
		}
	};
}
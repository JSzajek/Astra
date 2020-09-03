#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class BasicShader : public Shader
	{
	public:
		BasicShader(const char* filepath =
			"C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/shaders/Basic.shader")
			: Shader(filepath)
		{
		}
	};
}
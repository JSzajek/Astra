#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class BasicShader : public Shader
	{
	public:
		BasicShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/Basic.shader")
			: Shader(filepath)
		{
		}
	};
}
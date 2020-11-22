#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class SelectionShader : public Shader
	{
	public:
		SelectionShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/Selection.shader")
			: Shader(filepath, ShaderType::Basic)
		{
		}
	};
}
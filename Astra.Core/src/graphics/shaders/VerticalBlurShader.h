#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	#define TARGET_HEIGHT_TAG		"targetHeight"
	
	class VerticalBlurShader : public Shader
	{
	public:
		VerticalBlurShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/VerticalBlur.shader")
			: Shader(filepath, ShaderType::Basic)
		{
		}
	};
}
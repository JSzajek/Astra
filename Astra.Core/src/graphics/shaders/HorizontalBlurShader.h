#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	#define TARGET_WIDTH_TAG		"targetWidth"

	class HorizontalBlurShader : public Shader
	{
	public:
		HorizontalBlurShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/HorizontalBlur.shader")
			: Shader(filepath, ShaderType::Basic)
		{
		}
	};
}
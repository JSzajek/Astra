#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	#define TARGET_WIDTH_TAG		"targetWidth"

	class HorizontalBlurShader : public Shader
	{
	public:
		HorizontalBlurShader(const char* filepath =
			RESOURCE("res/shaders/HorizontalBlur.shader"))
			: Shader(filepath)
		{
		}
	};
}
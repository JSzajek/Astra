#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	#define TARGET_HEIGHT_TAG		"targetHeight"
	
	class VerticalBlurShader : public Shader
	{
	public:
		VerticalBlurShader(const char* filepath =
			RESOURCE("res/shaders/VerticalBlur.shader"))
			: Shader(filepath)
		{
		}
	};
}
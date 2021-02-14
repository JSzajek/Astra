#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	#define MODULATE_TAG			"modulateColor"

	class FontShader : public Shader
	{
	public:
		FontShader(const char* filepath =
			"../Astra/res/shaders/Font.shader")
			: Shader(filepath)
		{
		}
	};
}
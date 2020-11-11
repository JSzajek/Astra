#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	#define REGULAR_COLOR_MAP		"colorMap"
	#define BRIGHT_BLUR_MAP			"brightBlur"

	class SplitBloomShader : public Shader
	{
	public:
		SplitBloomShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/Bloom.shader")
			: Shader(filepath, ShaderType::Basic)
		{
		}
	};

	class CombineBloomShader : public Shader
	{
	public:
		CombineBloomShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/FinalBloom.shader")
			: Shader(filepath, ShaderType::Basic)
		{
		}
	};
}
#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	#define FAKE_LIGHT					"useFakeLighting"
	#define NUMBER_OF_ROWS				"numberOfRows"
	#define OFFSET_TAG					"offset"

	class BasicEntityShader : public Shader
	{
	public:
		BasicEntityShader(const char* filepath =
			RESOURCE("res/shaders/BasicEntity.shader")) : Shader(filepath)
		{
		}
	};
}
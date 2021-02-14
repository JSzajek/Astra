#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	#define MODEL_VIEW_PROJ_MATRIX_TAG		"mvpMatrix"
	#define NUMBER_OF_ROWS_TAG				"numberOfRows"
	#define OFFSET_TAG						"offset"
	
	class ShadowShader : public Shader
	{
	public:
		ShadowShader(const char* filepath =
			"../Astra/res/shaders/Shadow.shader")
			: Shader(filepath)
		{
		}
	};
}
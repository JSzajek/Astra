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
			"../Astra.Core/src/resources/shaders/Shadow.shader")
			: Shader(filepath, ShaderType::Shadows)
		{
		}
	};
}
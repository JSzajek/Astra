#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	#define MODEL_VIEW_MATRIX_TAG	"modelViewMatrix"
	#define TEX_OFFSET_1_TAG		"texOffset1"
	#define TEX_OFFSET_2_TAG		"texOffset2"
	#define TEX_COORD_INFO_TAG		"texCoordInfo"
	
	class ParticleShader : public Shader
	{
	public:
		ParticleShader(const char* filepath =
			RESOURCE("res/shaders/Particle.shader"))
			: Shader(filepath)
		{
		}
	};
}
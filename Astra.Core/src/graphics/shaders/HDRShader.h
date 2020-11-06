#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	#define	HDR_ACTIVE_TAG		"hdrActive"
	#define EXPOSURE_TAG		"exposure"

	class HDRShader : public Shader
	{
	public:
		HDRShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/HDR.shader")
			: Shader(filepath, ShaderType::Basic)
		{
		}
	};
}
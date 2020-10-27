#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class ShadowShader : public Shader
	{
	public:
		static constexpr const char* ModelViewProjMatrixTag = "mvpMatrix";
	public:
		ShadowShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/Shadow.shader")
			: Shader(filepath, ShaderType::Shadows)
		{
		}
	};
}
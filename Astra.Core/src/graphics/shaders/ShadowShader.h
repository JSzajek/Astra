#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class ShadowShader : public Shader
	{
	public:
		static constexpr const char* ModelViewProjMatrixTag = "mvpMatrix";
		static constexpr const char* NumberOfRowsTag = "numberOfRows";
		static constexpr const char* OffsetTag = "offset";
	public:
		ShadowShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/Shadow.shader")
			: Shader(filepath, ShaderType::Shadows)
		{
		}
	};
}
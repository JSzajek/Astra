#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class ParticleShader : public Shader
	{
	public:
		static constexpr const char* ModelViewMatrixTag = "modelViewMatrix";

		static constexpr const char* TexOffset1Tag = "texOffset1";
		static constexpr const char* TexOffset2Tag = "texOffset2";
		static constexpr const char* TexCoordInfoTag = "texCoordInfo";
	public:
		ParticleShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/Particle.shader")
			: Shader(filepath, ShaderType::Particles)
		{
		}
	};
}
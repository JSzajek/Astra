#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class ParticleShader : public Shader
	{
	public:
		static constexpr const char* ModelViewMatrixTag = "modelViewMatrix";
	public:
		ParticleShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/Particle.shader")
			: Shader(filepath, ShaderType::Particles)
		{
		}
	};
}
#pragma once

#include "Shader.h"
#include "../Window.h"

namespace Astra::Graphics
{
	class SkyboxShader : public Shader
	{
	public:
		static constexpr const char* FogColorTag = "fogColor";
		static constexpr const char* FirstCubeMapTag = "cubeMap";
		static constexpr const char* SecondCubeMapTag = "cubeMap2";
		static constexpr const char* BlendFactorTag = "blendFactor";

		static constexpr const float RotationSpeed = 1.0f;
	private:
		float m_rotation;
	public:
		SkyboxShader(const char* filepath =
			"C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/shaders/Skybox.shader")
			: Shader(filepath, ShaderType::Skybox), m_rotation(0)
		{
		}

		void SetUniformMat4(const GLchar* name, const Math::Mat4& matrix) override
		{
			// Should look into better way besides copying
			Math::Mat4 copy = Math::Mat4(matrix);
			copy.columns[3].x = 0;
			copy.columns[3].y = 0;
			copy.columns[3].z = 0;
			m_rotation += RotationSpeed * Window::delta;
			copy = copy.Rotate(m_rotation, Math::YAxis);
			Shader::SetUniformMat4(name, copy);
		}
	};
}
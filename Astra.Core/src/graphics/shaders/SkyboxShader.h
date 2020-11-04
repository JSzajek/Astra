#pragma once

#include "Shader.h"
#include "../Window.h"

namespace Astra::Graphics
{
	#define FIRST_CUBE_MAP_TAG		"cubeMap"
	#define SECOND_CUBE_MAP_TAG		"cubeMap2"
	#define BLEND_FACTOR_TAG		"blendFactor"

	#define RotationSpeed			1.0f

	class SkyboxShader : public Shader
	{
	public:
	private:
		float m_rotation;
	public:
		SkyboxShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/Skybox.shader")
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
			copy = copy.Rotate(m_rotation, Math::Vec3::Y_Axis);
			Shader::SetUniformMat4(name, copy);
		}
	};
}
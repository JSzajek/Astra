#include "BasicShader.h"

namespace Azura::Graphics
{
	BasicShader::BasicShader(const char* filepath)
		: Shader(filepath)
	{
	}

	void BasicShader::BindAttributes()
	{
		BindAttribute(0, "position");
		BindAttribute(1, "textureCoords");
		BindAttribute(2, "normal");
	}

	void BasicShader::GetUniformLocations()
	{
		m_projectionMatrixLocation = GetUniformLocation("projectionMatrix");
		m_viewMatrixLocation = GetUniformLocation("viewMatrix");
		m_transformMatrixLocation = GetUniformLocation("transformMatrix");
	}
}
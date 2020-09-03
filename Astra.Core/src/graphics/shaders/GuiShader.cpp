#include "GuiShader.h"

namespace Azura::Graphics
{
	GuiShader::GuiShader(const char* filepath)
		: m_transformMatrixLocation(GetUniformLocation("transformMatrix")), 
	{
	}

	void GuiShader::BindAttributes()
	{
		BindAttribute(0, "position");
	}

	void GuiShader::GetUniformLocations()
	{
		m_transformMatrixLocation = GetUniformLocation("transformMatrix");
	}
}
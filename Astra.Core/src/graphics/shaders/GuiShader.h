#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class GuiShader : public Shader
	{
	public:
		GuiShader(const char* filepath =
			"C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/shaders/Gui.shader")
			: Shader(filepath, ShaderType::Gui)
		{
		}
	};
}
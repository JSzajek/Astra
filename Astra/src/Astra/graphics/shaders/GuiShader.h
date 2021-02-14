#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class GuiShader : public Shader
	{
	public:
		GuiShader(const char* filepath =
			"../Astra/res/shaders/Gui.shader")
			: Shader(filepath)
		{
		}
	};
}
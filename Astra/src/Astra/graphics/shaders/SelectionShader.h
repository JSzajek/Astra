#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
#define SELECTION_COLOR_TAG		"selectionColor"

	class SelectionShader : public Shader
	{
	public:
		SelectionShader(const char* filepath =
			RESOURCE("res/shaders/Selection.shader"))
			: Shader(filepath)
		{
		}
	};
}
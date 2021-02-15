#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
#if ASTRA_DEBUG
	#define MODEL_VIEW_MATRIX_TAG	"modelViewMatrix"

	class GizmoShader : public Shader
	{
	public:
		GizmoShader(const char* filepath =
			RESOURCE("res/shaders/Gizmo.shader"))
			: Shader(filepath)
		{
		}
	};
#endif
}
#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
#if _DEBUG
	#define MODEL_VIEW_MATRIX_TAG	"modelViewMatrix"

	class GizmoShader : public Shader
	{
	public:
		GizmoShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/Gizmo.shader")
			: Shader(filepath, ShaderType::Gizmos)
		{
		}
	};
#endif
}
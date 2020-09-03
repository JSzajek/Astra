#pragma once

#include "../shaders/Shader.h"
#include "../buffers/VertexArray.h"

namespace Astra::Graphics
{
	class Renderer
	{
	protected:
		Shader* m_shader;
	public:
		void UpdateProjectionMatrix(const Math::Mat4& projectionMatrix);
	protected:
		Renderer(Shader* shader);
		~Renderer();

		void UnbindTextureModel();
		void UpdateDynamicVbo();
		void UpdateTexture();
		
		virtual void Draw(const Math::Mat4& viewMatrix) = 0;
	};
}
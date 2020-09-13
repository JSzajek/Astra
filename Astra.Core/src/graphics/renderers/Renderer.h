#pragma once

#include "../shaders/Shader.h"
#include "../buffers/VertexArray.h"
#include "../buffers/FrameBuffer.h"

namespace Astra::Graphics
{
	class Renderer
	{
	protected:
		Shader* m_shader;
	public:
		void UpdateProjectionMatrix(const Math::Mat4& projectionMatrix);
		void BindFrameBuffer(GLuint bufferId, unsigned int width, unsigned int height);
		void UnbindFrameBuffer();
	protected:
		Renderer(Shader* shader);
		~Renderer();

		void UnbindVertexArray();
		void UpdateDynamicVbo();
		void UpdateTexture();
		
		virtual void Draw(const Math::Mat4& viewMatrix) = 0;
	};
}
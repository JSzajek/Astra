#pragma once

#include "../shaders/Shader.h"
#include "../buffers/VertexArray.h"
#include "../buffers/FrameBuffer.h"

namespace Astra::Graphics
{
	class Renderer
	{
	public:
		static const Math::Vec4 DefaultClipPlane;
	protected:
		Shader* m_shader;
	public:
		void UpdateProjectionMatrix(const Math::Mat4& projectionMatrix);
		void BindFrameBuffer(GLuint bufferId, unsigned int width, unsigned int height);
		void UnbindFrameBuffer();

		
		inline virtual void Clear() { }
	protected:
		Renderer();
		~Renderer();

		virtual void SetShader(Shader* shader);
		virtual void UnbindVertexArray();
		void UpdateDynamicVbo();
		void UpdateTexture();
		
		virtual void Draw(const Math::Mat4* viewMatrix = NULL, const Math::Vec4& inverseViewVector = NULL, const Math::Vec4& clipPlane = DefaultClipPlane) = 0;
	};
}
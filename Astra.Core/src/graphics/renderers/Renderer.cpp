#include "Renderer.h"

#include "../Window.h"
#include "../../logger/Logger.h"

namespace Astra::Graphics
{
	const Math::Vec4 Renderer::DefaultClipPlane = Math::Vec4(0);

	Renderer::Renderer()
		: m_shader(NULL)
	{
	}

	Renderer::~Renderer()
	{
		delete m_shader;
	}

	void Renderer::SetShader(Shader* shader)
	{
		m_shader = shader;
	}

	void Renderer::UpdateProjectionMatrix(const Math::Mat4& projectionMatrix)
	{
		m_shader->Start();
		m_shader->SetUniformMat4(Shader::ProjectionMatrixTag, projectionMatrix);
		m_shader->Stop();
	}

	void Renderer::BindFrameBuffer(GLuint bufferId, unsigned int width, unsigned int height)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, bufferId);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::LogError("Frame Buffer Did Not Attach Correctly.");
		}
		glViewport(0, 0, width, height);
	}

	void Renderer::UnbindVertexArray()
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glDisableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glDisableVertexAttribArray(static_cast<unsigned short>(BufferType::TextureCoords));
		glDisableVertexAttribArray(static_cast<unsigned short>(BufferType::Normals));
		glDisableVertexAttribArray(static_cast<unsigned short>(BufferType::Tangents));
		glBindVertexArray(0);
	}

	void Renderer::UnbindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Window::width, Window::height);
		glFinish();
	}

	void Renderer::UpdateDynamicVbo()
	{
		//TODO: Implement
	}

	void Renderer::UpdateTexture()
	{
		//TODO: Implement
	}
}
#include "astra_pch.h"

#include "Renderer.h"
#include "Astra/Application.h"

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

	void Renderer::UpdateProjectionMatrix(const Math::Mat4* projectionMatrix)
	{
		m_shader->Start();
		m_shader->SetUniformMat4(PROJECTION_MATRIX_TAG, projectionMatrix);
		m_shader->Stop();
	}

	void Renderer::BindFrameBuffer(GLuint bufferId, unsigned int width, unsigned int height)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, bufferId);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			ASTRA_CORE_ERROR("Renderer Frame Buffer Did Not Attach Correctly.");
		}
		glViewport(0, 0, width, height);
	}

	void Renderer::UnbindVertexArray()
	{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
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
		glViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
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
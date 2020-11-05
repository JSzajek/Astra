#include "ScreenRenderer.h"

#include "../loaders/Loader.h"
#include "../Window.h"
#include "../../logger/Logger.h"

namespace Astra::Graphics
{
	ScreenRenderer::ScreenRenderer(Shader* shader)
	{
		Renderer::SetShader(shader);
		m_defaultQuad = Loader::Load(GL_TRIANGLE_STRIP, { -1, 1, -1, -1, 1, 1, 1, -1 }, 2);
		m_screenBuffer = Loader::LoadFrameBuffer(Window::width, Window::height);
	}

	ScreenRenderer::~ScreenRenderer()
	{
		delete m_defaultQuad;
		delete m_screenBuffer;
	}

	void ScreenRenderer::Attach() 
	{ 
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_screenBuffer->GetId());
		if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::LogError("Frame Buffer Did Not Attach Correctly.");
		}
		glViewport(0, 0, Window::width, Window::height);
	}
	
	void ScreenRenderer::Unattach() 
	{ 
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glViewport(0, 0, Window::width, Window::height);
	}

	void ScreenRenderer::Draw(const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane)
	{
		m_shader->Start();
		glBindVertexArray(m_defaultQuad->vaoId);
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glDisable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_screenBuffer->GetColorAttachment());

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(m_defaultQuad->drawType, 0, m_defaultQuad->vertexCount);

		glEnable(GL_DEPTH_TEST);
		glDisableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glBindVertexArray(0);
		m_shader->Stop();
	}
}
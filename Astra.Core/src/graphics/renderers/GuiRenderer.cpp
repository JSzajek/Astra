#include "GuiRenderer.h"
#include "../../math/Mat4Utils.h"

namespace Astra::Graphics
{
	GuiRenderer::GuiRenderer(GuiShader* shader)
		: Renderer((Shader*)shader)
	{
		m_defaultQuad = Loader::Load(GL_TRIANGLE_STRIP, { -1, 1, -1, -1, 1, 1, 1, -1 });
	}

	GuiRenderer::~GuiRenderer()
	{
		delete m_defaultQuad;
	}

	void GuiRenderer::Draw(const Math::Mat4& viewMatrix)
	{
		if (m_guis.size() <= 0) { return; }
		m_shader->Start();
		glBindVertexArray(m_defaultQuad->vaoId);
		glEnableVertexAttribArray(BufferType::Vertices);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		for (const GuiTexture* gui : m_guis)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gui->GetId());
			m_shader->SetUniformMat4(Shader::TransformMatrixTag, Math::Mat4Utils::Transformation(*gui));
			glDrawArrays(m_defaultQuad->drawType, 0, m_defaultQuad->vertexCount);
		}
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDisableVertexAttribArray(BufferType::Vertices);
		glBindVertexArray(0);
		m_shader->Stop();
	}
}
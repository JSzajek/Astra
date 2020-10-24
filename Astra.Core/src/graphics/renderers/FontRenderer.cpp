#include "FontRenderer.h"

namespace Astra::Graphics
{
	FontRenderer::FontRenderer(FontShader* shader)
		: Renderer((Shader*)shader)
	{
	}

	void FontRenderer::Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane)
	{
		if (m_texts.size() == 0) { return; }
		m_shader->Start();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		for (const auto& directory : m_texts)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, directory.first->GetTextureAtlas());
			for (const GuiText& text : directory.second)
			{
				RenderText(text);
			}
		}
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		UnbindVertexArray();
		m_shader->Stop();
	}

	void FontRenderer::RenderText(const GuiText& text)
	{
		glBindVertexArray(text.GetMesh());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		m_shader->SetUniform3f(FontShader::ColorTag, text.GetColor());
		m_shader->SetUniform2f(FontShader::TranslationTag, text.GetPosition());
		glDrawArrays(GL_TRIANGLES, 0, text.GetVertexCount());
	}
}
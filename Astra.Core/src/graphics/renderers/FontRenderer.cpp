#include "FontRenderer.h"

namespace Astra::Graphics
{
	FontRenderer::FontRenderer(FontShader* shader)
		: Renderer()
	{
		Renderer::SetShader(shader);
	}

	void FontRenderer::Draw(float delta, const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane)
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
			for (const GuiText* text : directory.second)
			{
				RenderText(text);
			}
		}
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		UnbindVertexArray();
		m_shader->Stop();
	}

	void FontRenderer::RenderText(const GuiText* text)
	{
		glBindVertexArray(text->GetMesh());
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::TextureCoords));

		m_shader->SetUniform3f(COLOR_TAG, text->Color);
		m_shader->SetUniform1f(WIDTH_TAG, text->FontWidth());
		m_shader->SetUniform1f(EDGE_TAG, text->FontEdge());

		m_shader->SetUniform3f(OUTLINE_COLOR_TAG, text->OutlineColor);
		m_shader->SetUniform1f(OUTLINE_WIDTH_TAG, text->OutlineWidth());
		m_shader->SetUniform1f(OUTLINE_EDGE_TAG, text->OutlineEdge());

		m_shader->SetUniform2f(TRANSLATION_TAG, text->Position);

		glDrawArrays(GL_TRIANGLES, 0, text->GetVertexCount());
	}
}
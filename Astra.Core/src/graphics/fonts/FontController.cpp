#include "FontController.h"

namespace Astra::Graphics
{
	FontController::FontController()
	{
		m_fontShader = new FontShader();
		m_fontRenderer = new FontRenderer(m_fontShader);
	}

	FontController::~FontController()
	{
		delete m_fontRenderer;
	}

	void FontController::RenderImpl()
	{
		m_fontRenderer->Draw(NULL, NULL);
	}

	void FontController::LoadTextImpl(GuiText& text)
	{
		FontType* font = text.GetFontType();
		const TextMeshData* data = font->LoadText(text);
		int vao = Loader::Load(GL_TRIANGLES, data->VertexPositions, data->TextureCoords);
		text.SetMeshInfo(vao, data->GetVertexCount());
		auto found = m_fontRenderer->GetTexts().find(font);
		if (found != m_fontRenderer->GetTexts().end())
		{
			found->second.push_back(text);
		}
		else
		{
			std::vector<GuiText> textBatch = std::vector<GuiText>();
			textBatch.push_back(text);
			m_fontRenderer->GetTexts().insert(std::make_pair(font, textBatch));
		}
		delete data; // Clean up
	}

	void FontController::RemoveTextImpl(GuiText& text)
	{
		auto found = m_fontRenderer->GetTexts().find(text.GetFontType());
		if (found != m_fontRenderer->GetTexts().end())
		{
			for (auto iter = found->second.begin(); iter != found->second.end(); iter++)
			{
				if (*iter == text)
				{
					iter = found->second.erase(iter);
				}
			}
			if (found->second.empty())
			{
				m_fontRenderer->GetTexts().erase(text.GetFontType());
			}
		}
	}

	void FontController::ClearImpl()
	{
		m_fontRenderer->Clear();
	}
}
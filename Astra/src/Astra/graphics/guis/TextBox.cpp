#include "astra_pch.h"

#include "TextBox.h"

#include "Astra/graphics/ResourceManager.h"
#include "Astra/graphics/loaders/Loader.h"

namespace Astra::Graphics
{
    TextBox::TextBox()
        : Gui(), m_font(NULL), m_vao(0), m_vbo(0)
    {
    }

    TextBox::TextBox(const char* name, const char* text, const FontAtlas* font, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
        : Gui(name, position, rotation, scale), m_font(font), m_vao(0), m_vbo(0)
    {
        m_rect.SetSize(Math::iVec2(0, m_font->GetFontSize()));
        SetText(text);
    }

    TextBox::TextBox(const char* text, const FontAtlas* font, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
        : Gui(position, rotation, scale), m_font(font), m_vao(0), m_vbo(0)
    {
        m_rect.SetSize(Math::iVec2(0, m_font->GetFontSize()));
        SetText(text);
    }

    TextBox::TextBox(const char* text, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
        : TextBox(text, ResourceManager::LoadFontAtlas(DEFAULT_FONT_PATH, DEFAULT_FONT_SIZE), position, rotation, scale)
    {
    }

    TextBox::TextBox(const char* name, const char* text, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
        : TextBox(name, text, ResourceManager::LoadFontAtlas(DEFAULT_FONT_PATH, DEFAULT_FONT_SIZE), position, rotation, scale)
    {
    }

    TextBox::~TextBox()
    {
        //RESOURCE_UNLOAD(m_font);
    }

    void TextBox::GenerateVertices(const std::string& string)
    {
        // TODO: Investigate better buffer object streaming techniques
        m_text = string;
        if (m_vao == 0)
        {
            glGenVertexArrays(1, &m_vao);
        }

        if (m_vbo == 0)
        {
            glGenBuffers(1, &m_vbo);
        }

        if (m_text.size() == 0)
        {
            return;
        }

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        auto vertices = std::vector<float>();
        vertices.reserve(m_text.size() * 4 * 6);
        std::string::const_iterator c;
        unsigned int cursorX = 0;

        for (c = m_text.begin(); c != m_text.end(); c++)
        {
            const auto& character = m_font->GetCharacter(*c);
            const auto flipped = character.GetFlipped();

            unsigned int w = character.GetWidth();
            unsigned int h = character.GetHeight();

            float xpos = static_cast<float>(cursorX + character.GetBearingX());
            float ypos = static_cast<float>(m_font->GetFontSize() - (character.GetBearingY() - h));

            float l_x = character.GetTexCoord(0);
            float r_x = character.GetTexCoord(1);
            float top = character.GetTexCoord(2);
            float bottom = character.GetTexCoord(3);

            vertices.push_back(xpos);
            vertices.push_back(ypos);
            vertices.push_back(flipped ? r_x : l_x);
            vertices.push_back(bottom);

            vertices.push_back(xpos + w);
            vertices.push_back(ypos - h);
            vertices.push_back(flipped ? l_x : r_x);
            vertices.push_back(top);

            vertices.push_back(xpos);
            vertices.push_back(ypos - h);
            vertices.push_back(l_x);
            vertices.push_back(flipped ? bottom : top);

            vertices.push_back(xpos);
            vertices.push_back(ypos);
            vertices.push_back(flipped ? r_x : l_x);
            vertices.push_back(bottom);

            vertices.push_back(xpos + w);
            vertices.push_back(ypos);
            vertices.push_back(r_x);
            vertices.push_back(flipped ? top : bottom);

            vertices.push_back(xpos + w);
            vertices.push_back(ypos - h);
            vertices.push_back(flipped ? l_x : r_x);
            vertices.push_back(top);

            cursorX += character.GetAdvance();
        }

        // update content of VBO memory
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 6 * m_text.size(), &vertices[0], GL_STREAM_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_rect.GetSize().x = cursorX;
    }
}
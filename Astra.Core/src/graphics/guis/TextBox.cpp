#include "TextBox.h"

#include "../loaders/Loader.h"

namespace Astra::Graphics
{
    void TextBox::GenerateVertices(const std::string& string)
    {
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        m_text = string;

        auto vertices = std::vector<float>();
        vertices.reserve(m_text.size() * 4 * 6);
        std::string::const_iterator c;
        float cursorX = 0;

        for (c = m_text.begin(); c != m_text.end(); c++)
        {
            const auto& character = m_font->GetCharacter(*c);// m_characters[*c];
            const auto flipped = character.GetFlipped();

            float w = character.GetWidth();
            float h = character.GetHeight();

            float xpos = cursorX + character.GetBearingX();
            float ypos = m_font->GetFontSize() - (character.GetBearingY() - h);// (character.GetSize().y - character.GetBearing().y);

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
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 6 * m_text.size(), &vertices[0], GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}
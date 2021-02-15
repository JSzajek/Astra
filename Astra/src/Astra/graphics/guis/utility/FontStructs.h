#pragma once

#include "../../../math/iVec2.h"
#include "../../../math/iVec4.h"
#include "../../../math/Vec4.h"

namespace Astra::Graphics
{
    struct Pre_Char
    {
    private:
        unsigned char m_character;
        Math::iVec2 m_bearing;
        Math::iVec2 m_size;
        Math::iVec2 m_offset;
        bool m_flipped;
        unsigned int m_advance;
    public:
        Pre_Char()
            : m_character(0), m_bearing(0), m_size(0), m_offset(0), m_advance(0), m_flipped(false)
        {
        }

        Pre_Char(const unsigned char ch, const int w, const int h, const int bearingX, const int bearingY, const int advance)
            : m_character(ch), m_bearing(bearingX, bearingY), m_size(w, h), m_offset(0), m_advance(advance), m_flipped(false)
        {
        }

        inline void SetOffset(int x, int y) { m_offset = Math::iVec2(x, y); }
        inline void SetFlipped(bool flipped) { m_flipped = flipped; }

        inline const char GetChar() const { return m_character; }
        inline const Math::iVec2& GetBearing() const { return m_bearing; }
        inline const Math::iVec2& GetSize() const { return m_size; }
        inline const Math::iVec2& GetOffset() const { return m_offset; }
        inline const unsigned int GetAdvance() const { return m_advance; }
        inline const bool GetFlipped() const { return m_flipped; }
    };

    struct Post_Char
    {
    private:
        char m_character;
        unsigned int m_advance;
        bool m_flipped;
        Math::iVec4 m_information;    // <width, height, xBearing, yBearing>
        Math::Vec4 m_texCoordinates; // <left, right, top, bottom>
    public:
        Post_Char()
            : m_character(0), m_advance(0), m_information(0), m_texCoordinates(0), m_flipped(0)
        {
        }

        Post_Char(const Pre_Char& pre, unsigned int texWidth, unsigned int texHeight)
            : m_character(pre.GetChar()), m_advance(pre.GetAdvance() >> 6), m_flipped(pre.GetFlipped())
        {
            m_information = Math::iVec4(pre.GetSize(), pre.GetBearing());

            float w = static_cast<float>(pre.GetSize().x);
            float h = static_cast<float>(pre.GetSize().y);

            float atlasWidth = static_cast<float>(texWidth);
            float atlasHeight = static_cast<float>(texHeight);

            float left = pre.GetOffset().x / atlasWidth;
            float right = left + (pre.GetFlipped() ? (h / atlasHeight) : (w / atlasWidth));

            float top = pre.GetOffset().y / atlasHeight;
            float bottom = top + (pre.GetFlipped() ? (w / atlasWidth) : (h / atlasHeight));

            m_texCoordinates = Math::Vec4(left, right, top, bottom);
        }

        inline const unsigned int GetWidth() const { return m_information[0]; }
        inline const unsigned int GetHeight() const { return m_information[1]; }
        inline const unsigned int GetBearingX() const { return m_information[2]; }
        inline const unsigned int GetBearingY() const { return m_information[3]; }

        inline const bool GetFlipped() const { return m_flipped; }

        inline const float GetTexCoord(size_t index) const { return m_texCoordinates[index]; }
        inline const unsigned int GetAdvance() const { return m_advance; }
    };
}
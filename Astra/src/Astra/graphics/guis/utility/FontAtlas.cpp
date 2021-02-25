#include "astra_pch.h"

#include "FontAtlas.h"
#include <GL/glew.h>

namespace Astra::Graphics
{
    FontAtlas::FontAtlas()
        : m_fontSize(0), id(0)
    #if ASTRA_DEBUG
        , m_filePath()
    #endif
    {
    }

	FontAtlas::FontAtlas(const char* const fontFile, const unsigned int fontSize)
        : m_fontSize(fontSize)
    #if ASTRA_DEBUG
        , m_filePath(fontFile)
    #endif
    {
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
            ASTRA_CORE_ERROR("FreeType: Could Not Initialize Library.");
			return;
		}

		FT_Face face;
		if (FT_New_Face(ft, fontFile, 0, &face))
		{
            ASTRA_CORE_ERROR("FreeType: Failed to Load Font at: {0}", fontFile);
			return;
		}

		// Set pixel sizes
		FT_Set_Pixel_Sizes(face, 0, fontSize);

        // Initialize texture
		LoadFontTextureAtlas(fontFile, fontSize, face);

		// Clean up the face
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

    FontAtlas::~FontAtlas()
    {
        glDeleteTextures(1, &id);
    }

    const Post_Char& FontAtlas::GetCharacter(char c) const
    {
        // TODO: Improve miss checking
        auto found = m_characters.find(c);
        if (found == m_characters.end())
        {
            ASTRA_CORE_WARN("FontAtlas: Does Not Contain Character: {0}", c);
        }
        return found->second;
    }

    void FontAtlas::LoadCharacter(const FT_Face& face, unsigned char c)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            ASTRA_CORE_WARN("FontAtlas: Could Not Load Glyph: {0}", c);
        }
    }

	void FontAtlas::LoadFontTextureAtlas(const char* const filepath, unsigned int fontSize, const FT_Face& face)
	{
		std::vector<rect_type> rectangles;
		std::unordered_map<Math::iVec2, std::stack<char>, iVec2Hasher> m_possible;
        std::unordered_map<char, Pre_Char> characters;

		static const auto report_successful = [](rect_type&) {
			return rectpack2D::callback_result::CONTINUE_PACKING;
		};

		static const auto report_unsuccessful = [](rect_type&) {
			return rectpack2D::callback_result::ABORT_PACKING;
		};

        for (unsigned char c = 32; c < 128; c++)
        {
            LoadCharacter(face, c);

            Math::iVec2 size = Math::iVec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
            auto character = Pre_Char(c, size.x, size.y, face->glyph->bitmap_left, face->glyph->bitmap_top, face->glyph->advance.x);
            unsigned int padding = size.x * size.y > 0 ? 2 * PADDING_AMOUNT : 0;
            
            auto rect = rectpack2D::rect_xywh(0, 0, size.x + padding, size.y + padding);
            rectangles.emplace_back(rect);
            characters[c] = character;

            if (m_possible.find(size) == m_possible.end())
            {
                m_possible[size] = std::stack<char>();
            }
            m_possible[size].push(c);
        }

        const auto result_size = rectpack2D::find_best_packing<spaces_type>(rectangles, make_finder_input(MAX_SIDE, DISCARD_STEP,
                                                                            report_successful, report_unsuccessful, runtime_flipping_mode));
        std::vector<unsigned char> imgBuffer(result_size.area(), 0);
        unsigned int padding = (2 * PADDING_AMOUNT);
        bool flipped = 0;
        unsigned char c;
        for (const auto& rect : rectangles)
        {
            Math::iVec2 size = rect.area() == 0 ? Math::iVec2(rect.w, rect.h) : Math::iVec2(rect.w - padding, rect.h - padding);

            auto found = m_possible.find(size);
            if (found != m_possible.end())
            {
                if (found->second.empty())
                {
                    found = m_possible.find(Math::iVec2(rect.h - padding, rect.w - padding));
                    c = found->second.top();
                    found->second.pop();

                    characters[c].SetFlipped(true);
                }
                else
                {
                    c = found->second.top();
                    found->second.pop();
                }
            }
            else
            {
                // Rectangle has been flipped
                found = m_possible.find(Math::iVec2(rect.h - padding, rect.w - padding));
                c = found->second.top();
                found->second.pop();
                
                characters[c].SetFlipped(true);
            }

            unsigned int xOffset = rect.x + PADDING_AMOUNT;
            unsigned int yOffset = rect.y + PADDING_AMOUNT;
            characters[c].SetOffset(xOffset, yOffset);

            const auto& character = characters[c];
            LoadCharacter(face, character.GetChar());
            for (int sx = 0; sx < character.GetSize().x; sx++)
            {
                for (int sy = 0; sy < character.GetSize().y; sy++)
                {
                #if FLIPPING
                    if (character.GetFlipped())
                    {
                        imgBuffer[(xOffset + sy + (result_size.w * (yOffset + sx)))] = face->glyph->bitmap.buffer[face->glyph->bitmap.width - 1 - sx + (face->glyph->bitmap.width * sy)];
                    }
                    else
                    {
                        imgBuffer[(xOffset + sx + (result_size.w * (yOffset + sy)))] = face->glyph->bitmap.buffer[sx + (face->glyph->bitmap.width * sy)];
                    }
                #else 
                    imgBuffer[(xOffset + sx + (result_size.w * (yOffset + sy)))] = face->glyph->bitmap.buffer[sx + (face->glyph->bitmap.width * sy)];
                #endif
                }
            }

            m_characters[character.GetChar()] = Post_Char(character, result_size.w, result_size.h);
        }
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        width = result_size.w;
        height = result_size.h;
        
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, &imgBuffer[0]);

        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // Reset unpacking alignment to default

        glBindTexture(GL_TEXTURE_2D, 0);
	}
}
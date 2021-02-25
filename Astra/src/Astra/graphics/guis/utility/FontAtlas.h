#pragma once

#include <unordered_map>
#include <stack>

#include "FontStructs.h"

#include <ft2build.h>
#include <freetype/ftbitmap.h>
#include FT_FREETYPE_H

#include <rectpack2D/finders_interface.h>

namespace Astra::Graphics
{
    #define FLIPPING            1
	#define PADDING_AMOUNT		1
    #define MAX_SIDE            1000
    #define DISCARD_STEP        -4

    constexpr bool allow_flip = FLIPPING;
    const auto runtime_flipping_mode = FLIPPING ? rectpack2D::flipping_option::ENABLED : rectpack2D::flipping_option::DISABLED;

    using spaces_type = rectpack2D::empty_spaces<allow_flip, rectpack2D::default_empty_spaces>;
    using rect_type = rectpack2D::output_rect_t<spaces_type>;

	class FontAtlas
	{
    private:
        std::unordered_map<char, Post_Char> m_characters;
    private:
    #if ASTRA_DEBUG
        std::string m_filePath;
    #endif
        unsigned int width, height;
        unsigned int id;
        unsigned int m_fontSize;
	public:
		FontAtlas();
		FontAtlas(const char* const fontFile, const unsigned int fontSize);
        ~FontAtlas();

        inline unsigned int GetFontSize() const { return m_fontSize; }
        inline unsigned int GetId() const { return id; }

        const Post_Char& GetCharacter(char c) const;
    private:
        void LoadCharacter(const FT_Face& face, unsigned char c);
        void LoadFontTextureAtlas(const char* const filepath, unsigned int fontSize, const FT_Face& face);
    };

    struct iVec2Hasher
    {
        std::size_t operator()(const Math::iVec2& k) const
        {
            return ((17 * 31 + std::hash<int>()(k.x)) * 31 + std::hash<int>()(k.y));
        }
    };
}
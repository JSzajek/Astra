#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class FontShader : public Shader
	{
	public:
		static constexpr const char* TranslationTag = "translation";
		
		static constexpr const char* FontAtlasTag = "fontAtlas";
		
		static constexpr const char* ColorTag = "color";
		static constexpr const char* WidthTag = "width";
		static constexpr const char* EdgeTag = "edge";

		static constexpr const char* OutlineColorTag = "outlineColor";
		static constexpr const char* OutlineWidthTag = "outlineWidth";
		static constexpr const char* OutlineEdgeTag = "outlineEdge";
		
		static constexpr const char* OffsetTag = "offset";
	public:
		FontShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/Font.shader")
			: Shader(filepath, ShaderType::Gui)
		{
		}
	};
}
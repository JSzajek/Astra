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

	public:
		FontShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/Font.shader")
			: Shader(filepath, ShaderType::Gui)
		{
		}
	};
}
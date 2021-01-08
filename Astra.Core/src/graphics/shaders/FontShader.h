#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	#define TRANSLATION_TAG			"translation"
	#define FONT_ATLAS_TAG			"fontAtlas"
	
	#define COLOR_TAG				"color"
	#define WIDTH_TAG				"width"
	#define EDGE_TAG				"edge"
	
	#define OUTLINE_COLOR_TAG		"outlineColor"
	#define OUTLINE_WIDTH_TAG		"outlineWidth"
	#define OUTLINE_EDGE_TAG		"outlineEdge"
	
	#define OFFSET_TAG				"offset"
	
	class FontShader : public Shader
	{
	public:
		FontShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/Font.shader")
			: Shader(filepath)
		{
		}
	};
}
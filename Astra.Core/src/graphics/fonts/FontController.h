#pragma once

#include <vector>
#include <unordered_map>

#include "TextMesh.h"
#include "../loaders/Loader.h"
#include "../shaders/FontShader.h"
#include "../renderers/FontRenderer.h"

namespace Astra::Graphics
{
	class FontController
	{
	private:
		FontShader* m_fontShader;
		FontRenderer* m_fontRenderer;
	public:
		FontController(const FontController&) = delete;
		void operator=(const FontController&) = delete;

		static FontController& Get()
		{
			static FontController instance;
			return instance;
		}

		static void Render()
		{
			Get().RenderImpl();
		}

		static void LoadText(GuiText* text)
		{
			Get().LoadTextImpl(text);
		}

		static void RemoveText(GuiText* text)
		{
			Get().RemoveTextImpl(text);
		}

		static void Clear()
		{
			Get().ClearImpl();
		}

	private:
		FontController();
		~FontController();

		void RenderImpl();
		void LoadTextImpl(GuiText* text);
		void RemoveTextImpl(GuiText* text);
		void ClearImpl();
	};
}
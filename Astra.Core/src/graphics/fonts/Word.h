#pragma once

#include <vector>

#include "Character.h"

namespace Astra::Graphics
{
	struct Word
	{
	private:
		std::vector<const Character*> characters;
		double width;
		double fontSize;
	public:
		Word(double fontSize)
			: fontSize(fontSize), width(0)
		{
		}

		void AddCharacter(const Character* character) 
		{ 
			characters.push_back(character); 
			width += character->GetXAdvance() * fontSize;
		}

		inline const std::vector<const Character*>& GetCharacters() const { return characters; }
		inline const double GetWidth() const { return width; }
		inline const double GetFontSize() const { return fontSize; }
	};
}
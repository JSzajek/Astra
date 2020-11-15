#pragma once

#include <vector>

#include "Word.h"

namespace Astra::Graphics
{
	struct Line
	{
	private:
		std::vector<Word> words;
		float maxLength;
		float spaceSize;
		float currentLength;

	public:
		Line(float spaceWidth, float fontSize, float maxLength)
			: maxLength(maxLength), spaceSize(spaceWidth * fontSize), currentLength(0)
		{
		}
		
		bool AddWord(const Word& word)
		{
			float addedLength = word.GetWidth();
			addedLength += words.size() != 0 ? spaceSize : 0;
			if (currentLength + addedLength <= maxLength)
			{
				words.push_back(word);
				currentLength += addedLength;
				return true;
			}
			return false;
		}

		inline const std::vector<Word>& GetWords() const { return words; }
		inline const float GetLineLength() const { return currentLength; }
		inline const float GetMaxLength() const { return maxLength; }
	};
}
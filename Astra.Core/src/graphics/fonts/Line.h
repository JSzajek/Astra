#pragma once

#include <vector>

#include "Word.h"

namespace Astra::Graphics
{
	struct Line
	{
	private:
		std::vector<Word> words;
		double maxLength;
		double spaceSize;
		double currentLength;

	public:
		Line(double spaceWidth, double fontSize, double maxLength)
			: maxLength(maxLength), spaceSize(spaceWidth * fontSize), currentLength(0)
		{
		}
		
		bool AddWord(const Word& word)
		{
			double addedLength = word.GetWidth();
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
		inline const double GetLineLength() const { return currentLength; }
		inline const double GetMaxLength() const { return maxLength; }
	};
}
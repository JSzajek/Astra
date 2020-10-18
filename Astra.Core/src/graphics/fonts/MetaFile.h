#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "Character.h"
#include "../Window.h"
#include "../../utils/StringUtils.h"

namespace Astra::Graphics
{
	#define PAD_TOP				0
	#define PAD_LEFT			1
	#define PAD_BOTTOM			2
	#define PAD_RIGHT			3

	#define DESIRED_PADDING		3
	#define SPLITTER			' '
	#define NUMBER_SEPERATOR	','

	class MetaFile
	{
	private:
		double aspectRatio;
		double verticalPerPixelSize;
		double horizontalPerPixelSize;
		double spaceWidth;

		std::vector<int> padding;
		int paddingWidth;
		int paddingHeight;

		std::ifstream m_file;
		std::unordered_map<int, const Character*> metaData;
		std::unordered_map<std::string, std::string> values;
	public:
		MetaFile(const char* filepath);
		inline const double GetSpaceWidth() const { return spaceWidth; }
		inline const Character* GetCharacter(int ascii) const { return (*metaData.find(ascii)).second; }
	private:
		inline int GetValueOfVariable(const std::string& variable) { return std::stoi((*values.find(variable)).second); }

		bool ProcessNextLine();
		std::vector<int> GetValuesOfVariable(const std::string& variable);
		void LoadPaddingData();
		void LoadLineSizes();
		void LoadCharacterData(int imageWidth);
		const Character* LoadCharacter(int imageSize);
	};
}
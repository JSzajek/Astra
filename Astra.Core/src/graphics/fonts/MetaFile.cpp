#include "MetaFile.h"
#include "TextMesh.h"

namespace Astra::Graphics
{
	MetaFile::MetaFile(const char* filepath)
		: m_file(filepath), aspectRatio(Window::width / Window::height)
	{
		LoadPaddingData();
		LoadLineSizes();
		int img_width = GetValueOfVariable("scaleW");
		LoadCharacterData(img_width);

		m_file.close();
	}

	bool MetaFile::ProcessNextLine()
	{
		values.clear();
		std::string line;
		getline(m_file, line);
		if (line.empty()) { return false; }

		for (std::string part : StringUtils::Split(line, SPLITTER))
		{
			std::vector<std::string> valuePairs = StringUtils::Split(part, '=');
			if (valuePairs.size() == 2)
			{
				values.insert(std::make_pair(std::string(valuePairs[0]), std::string(valuePairs[1])));
			}
		}
	}

	std::vector<int> MetaFile::GetValuesOfVariable(const std::string& variable)
	{
		std::vector<std::string> numbers = StringUtils::Split((*values.find(variable)).second, NUMBER_SEPERATOR);
		std::vector<int> actualValues;
		actualValues.reserve(numbers.size());

		for (int i = 0; i < numbers.size(); i++)
		{
			actualValues.push_back(std::stoi(numbers[i]));
		}
		return actualValues;
	}

	void MetaFile::LoadPaddingData()
	{
		ProcessNextLine();
		padding = GetValuesOfVariable("padding");
		paddingWidth = padding[PAD_LEFT] + padding[PAD_RIGHT];
		paddingHeight = padding[PAD_TOP] + padding[PAD_BOTTOM];
	}

	void MetaFile::LoadLineSizes()
	{
		ProcessNextLine();
		int lineHeightPixels = GetValueOfVariable("lineHeight") - paddingHeight;
		verticalPerPixelSize = LINE_HEIGHT / (double)lineHeightPixels;
		horizontalPerPixelSize = verticalPerPixelSize / aspectRatio;
	}

	void MetaFile::LoadCharacterData(int imageWidth)
	{
		ProcessNextLine();
		ProcessNextLine();

		while (ProcessNextLine())
		{
			const Character* c = LoadCharacter(imageWidth);
			if (c != NULL)
			{
				metaData.insert(std::make_pair(c->Id(), c));
			}
		}
	}

	const Character* MetaFile::LoadCharacter(int imageSize)
	{
		int id = GetValueOfVariable("id");
		if (id == SPACE_ASCII)
		{
			spaceWidth = (GetValueOfVariable("xadvance") - paddingWidth) * horizontalPerPixelSize;
			return NULL;
		}
		double xTex = ((double)GetValueOfVariable("x") + (padding[PAD_LEFT] - DESIRED_PADDING)) / imageSize;
		double yTex = ((double)GetValueOfVariable("y") + (padding[PAD_TOP] - DESIRED_PADDING)) / imageSize;
		
		int width = GetValueOfVariable("width") - (paddingWidth - (2 * DESIRED_PADDING));
		int height = GetValueOfVariable("height") - (paddingHeight - (2 * DESIRED_PADDING));

		double quadWidth = width * horizontalPerPixelSize;
		double quadHeight = height * verticalPerPixelSize;

		double xTexSize = (double)width / (double)imageSize;
		double yTexSize = (double)height / (double)imageSize;
		
		double xOffset = (GetValueOfVariable("xoffset") + padding[PAD_LEFT] - DESIRED_PADDING) * horizontalPerPixelSize;
		double yOffset = (GetValueOfVariable("yoffset") + padding[PAD_TOP] - DESIRED_PADDING) * verticalPerPixelSize;
		double xAdvance = (GetValueOfVariable("xadvance") - paddingWidth) * horizontalPerPixelSize;
		return new Character(id, xTex, yTex, xTexSize, yTexSize, xOffset, yOffset, quadWidth, quadHeight, xAdvance);
	}
}
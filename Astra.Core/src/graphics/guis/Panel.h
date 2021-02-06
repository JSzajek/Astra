#pragma once

#include "Gui.h"
#include "Color.h"
#include "../fonts/MetaFile.h"

namespace Astra::Graphics
{
	class Panel : public Gui
	{
	private:
		unsigned int m_vao;
		unsigned int m_vbo;
		Math::Vec2 m_size;
	public:
		Panel(const GuiMaterial* material, const Math::Vec2& position, const Math::Vec2& scale, float rotation, MetaFile* metadata);

		inline const bool HasCustomVao() const override { return true; }
		inline const int GetCustomVao() const override { return m_vao; }
		inline const int GetCustomVbo() const override { return m_vbo; }

		/*Panel(const Color& color, const Math::Vec2& position, const Math::Vec2& scale, float rotation)
			: 
		{
			SetType(GuiType::Panel);
		}*/
		inline const Math::Vec2& GetSize() const override { return m_size; }

	private:
		void AddVerticesForCharacter(float xCursor, float yCursor, const Character* character, std::vector<float>& vertices)
		{
			float x = xCursor + (character->GetXOffset());
			float y = yCursor + (character->GetYOffset());
			float xMax = x + (character->GetXSize());
			float yMax = y + (character->GetYSize());
			float xProper = (2 * x) - 1;
			float yProper = (-2 * y) + 1;
			float xMaxProper = (2 * xMax) - 1;
			float yMaxProper = (-2 * yMax) + 1;
			AddVertices(vertices, xProper, yProper, xMaxProper, yMaxProper);
		}

		void AddVertices(std::vector<float>& vertices, float x, float y, float maxX, float maxY)
		{
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(x);
			vertices.push_back(maxY);
			vertices.push_back(maxX);
			vertices.push_back(maxY);
			vertices.push_back(maxX);
			vertices.push_back(maxY);
			vertices.push_back(maxX);
			vertices.push_back(y);
			vertices.push_back(x);
			vertices.push_back(y);
		}

		void AddTexCoords(std::vector<float>& texCoords, float x, float y, float maxX, float maxY)
		{
			texCoords.push_back(x);
			texCoords.push_back(y);
			texCoords.push_back(x);
			texCoords.push_back(maxY);
			texCoords.push_back(maxX);
			texCoords.push_back(maxY);
			texCoords.push_back(maxX);
			texCoords.push_back(maxY);
			texCoords.push_back(maxX);
			texCoords.push_back(y);
			texCoords.push_back(x);
			texCoords.push_back(y);
		}
	};
}
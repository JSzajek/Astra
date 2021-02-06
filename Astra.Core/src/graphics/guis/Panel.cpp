#include "Panel.h"
#include <GL/glew.h>
#include <vector>
#include "../../math/Mat4Utils.h"

namespace Astra::Graphics
{
	Panel::Panel(const GuiMaterial* material, const Math::Vec2& position, const Math::Vec2& scale, float rotation, MetaFile* metadata)
		: Gui(material, position, rotation, scale), m_size(15, 10)
	{
		unsigned int VBO;
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &VBO);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		std::string tempString = "OpenGL!";

		std::string::const_iterator c;
		std::vector<float> vertices;
		float xCursor = 0;
		float yCursor = 0;
		std::vector<float> textureCoords;
		for (c = tempString.begin(); c != tempString.end(); c++)
		{
			int ascii = static_cast<int>(*c);
			auto* character = metadata->GetCharacter(ascii);

			float xpos = xCursor + (character->GetXOffset());
			float ypos = yCursor - (character->GetYSize() - character->GetYOffset());
			float w = character->GetXSize();
			float h = character->GetYSize();

			vertices.push_back(xpos);
			vertices.push_back(ypos + h);
			vertices.push_back(0);
			vertices.push_back(0);
			vertices.push_back(xpos);
			vertices.push_back(ypos);
			vertices.push_back(0);
			vertices.push_back(1);
			vertices.push_back(xpos + w);
			vertices.push_back(ypos);
			vertices.push_back(1);
			vertices.push_back(1);
			vertices.push_back(xpos);
			vertices.push_back(ypos + h);
			vertices.push_back(0);
			vertices.push_back(0);
			vertices.push_back(xpos + w);
			vertices.push_back(ypos);
			vertices.push_back(1);
			vertices.push_back(1);
			vertices.push_back(xpos + w);
			vertices.push_back(xpos + h);
			vertices.push_back(1);
			vertices.push_back(0);

			xCursor += character->GetXAdvance();
		}
		glBufferData(GL_ARRAY_BUFFER, tempString.size() * sizeof(float) * 6 * 4, &vertices[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, 180, NULL, GL_STREAM_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 96, (void*)0);							// Transform Matrix
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 96, (void*)(1 * 4 * sizeof(float)));	// Transform Matrix
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 96, (void*)(2 * 4 * sizeof(float)));	// Transform Matrix
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 96, (void*)(3 * 4 * sizeof(float)));	// Transform Matrix
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 96, (void*)(4 * 4 * sizeof(float)));	// Texture Info
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 96, (void*)(5 * 4 * sizeof(float)));	// Modulate Color

		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		// Determine whether subbuffer once or per offset is more efficient
		Math::Mat4 transform = Math::Mat4Utils::Transformation(*this);
		glBufferSubData(GL_ARRAY_BUFFER, 1 * 96, sizeof(Math::Mat4), transform.data);
		float dat[8] =
		{
			static_cast<float>(0),
			static_cast<float>(1),
			static_cast<float>(0),
			static_cast<float>(0),
			1.0f,
			1.0f,
			1.0f,
			1.0f
		};
		glBufferSubData(GL_ARRAY_BUFFER, (1 * 96) + sizeof(Math::Mat4), sizeof(dat), dat);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}
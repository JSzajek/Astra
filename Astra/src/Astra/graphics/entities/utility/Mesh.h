#pragma once

#include <string>
#include <vector>

#include "Astra/math/Maths.h"

namespace Astra::Graphics
{
	struct Vertex
	{
		Math::Vec3 Position;
		Math::Vec3 Normal;
		Math::Vec2 TextureCoords;
	};

	struct Tex
	{
		unsigned int id;
		std::string type;
	};

	class Mesh
	{
	private:
		unsigned int m_vao, m_vbo, m_ebo;
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		std::vector<Tex> m_textures;
	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Tex>& textures);
	private:
		void Initialize();
	};
}
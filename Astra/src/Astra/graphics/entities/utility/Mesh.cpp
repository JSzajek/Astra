#include "astra_pch.h"

#include "Mesh.h"
#include "Astra/graphics/ResourceManager.h"

#include <GL/glew.h>

namespace Astra::Graphics
{
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<int>& indices, ImageMaterial* material)
		: m_material(material), m_vertexCount(indices.size())
	{
		Initialize(vertices, indices);
	}

	Mesh::Mesh(const std::vector<NormalVertex>& vertices, const std::vector<int>& indices, ImageMaterial* material)
		: m_material(material), m_vertexCount(indices.size())
	{
		Initialize(vertices, indices);
	}

	Mesh::Mesh(const Mesh& other)
		: m_vertexCount(other.m_vertexCount), 
			m_vao(other.m_vao), m_vbo(other.m_vbo), m_ebo(other.m_ebo),
			m_material(other.m_material)
	{
	}

	void Mesh::Initialize(const std::vector<Vertex>& vertices, const std::vector<int>& indices)
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);
		
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		
		// Setup Base Information
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoords));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		
		// Setup Animation Information
		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 3, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, BoneIds));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Weights));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void Mesh::Initialize(const std::vector<NormalVertex>& vertices, const std::vector<int>& indices)
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(NormalVertex), &vertices[0], GL_STATIC_DRAW);

		// Setup Base Information
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(NormalVertex), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(NormalVertex), (void*)offsetof(NormalVertex, TextureCoords));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(NormalVertex), (void*)offsetof(NormalVertex, Normal));
		
		// Setup Animation Information
		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 3, GL_INT, sizeof(NormalVertex), (void*)offsetof(NormalVertex, BoneIds));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(NormalVertex), (void*)offsetof(NormalVertex, Weights));
		
		// Setup Normal Mapping Information
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(NormalVertex), (void*)offsetof(NormalVertex, Tangent));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void Mesh::Unload()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);

		//RESOURCE_UNLOAD(m_material); // Reinstate
	}
}
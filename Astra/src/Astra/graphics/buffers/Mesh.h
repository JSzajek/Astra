#pragma once

#include <string>
#include <vector>
#include <map>

#include "Astra/math/Maths.h"
#include "Astra/graphics/buffers/Vertex.h"
#include "Astra/graphics/entities/animation/BoneInfo.h"
#include "Astra/graphics/materials/ImageMaterial.h"

namespace Astra::Graphics
{
	#define ASTRA_GL_TRIANGLES		0x0004
	#define ASTRA_TRIANGLE_STRIP	0x0005

	class Mesh
	{
	private:
#if ASTRA_DEBUG
		std::vector<Vertex> m_vertices;
		std::vector<NormalVertex> m_normVertices;
		std::vector<unsigned int> m_indices;
		float m_simplificationLevel;
#endif 
	private:
		unsigned int m_vertexCount;
		unsigned int m_drawtype;
	private:
		unsigned int m_vao, m_vbo, m_ebo;
	public:
		Mesh();
		Mesh(unsigned int drawtype, const std::vector<float>* vertices, unsigned int dimensions);
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int drawtype = ASTRA_GL_TRIANGLES);
		Mesh(const std::vector<NormalVertex>& vertices, const std::vector<unsigned int>& indices, unsigned int drawtype = ASTRA_GL_TRIANGLES);
		Mesh(const Mesh& other);
		~Mesh();

		inline unsigned int GetVAO() const { return m_vao; }
		inline unsigned int GetDrawType() const { return m_drawtype; }
		inline unsigned int GetVertexCount() const { return m_vertexCount; }
	#if ASTRA_DEBUG
		inline unsigned int GetIndicesCount() const { return m_indices.size(); }
		inline unsigned int GetVerticesCount() const { return m_vertices.size(); }

		void SimplifyMesh(float ratio);
	#endif
	private:
		void Initialize(const std::vector<float>* vertices, unsigned int dimensions);
		void Initialize(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		void Initialize(const std::vector<NormalVertex>& vertices, const std::vector<unsigned int>& indices);
	};

	struct MeshCreationSpec
	{
		unsigned int drawtype = 0;
		const std::vector<float>* vertices = NULL;
		unsigned int dimensions = 0;

		std::string filepath;
		void* mesh = NULL;
		const void* scene = NULL;
		std::map<std::string, BoneInfo>* map = NULL;
		int* counter = NULL;
		bool normalMapped = false;

		MeshCreationSpec(const std::string& filepath, unsigned int drawtype, const std::vector<float>* vertices, unsigned int dimensions)
			: filepath(filepath), drawtype(drawtype), vertices(vertices), dimensions(dimensions)
		{
		}

		MeshCreationSpec(const std::string& filepath, void* mesh, const void* scene,
						 std::map<std::string, BoneInfo>* map, int* counter, bool normalMapped)
			: filepath(filepath), mesh(mesh), scene(scene), map(map), counter(counter), normalMapped(normalMapped)
		{
		}
	};
}
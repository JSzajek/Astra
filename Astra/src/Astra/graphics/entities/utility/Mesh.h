#pragma once

#include <string>
#include <vector>
#include <map>

#include "Astra/math/Maths.h"
#include "Astra/graphics/entities/utility/BoneInfo.h"
#include "Astra/graphics/materials/ImageMaterial.h"

namespace Astra::Graphics
{
	#define MAX_BONES			75
	#define MAX_BONE_INFLUENCE	3

	struct Vertex
	{
		// Base Information
		Math::Vec3 Position;
		Math::Vec2 TextureCoords;
		Math::Vec3 Normal;

		// Animation Information
		int BoneIds[MAX_BONE_INFLUENCE] = { -1, -1, -1 };
		float Weights[MAX_BONE_INFLUENCE] = { 0.0f, 0.0f, 0.0f };
	};

	struct NormalVertex : public Vertex
	{
		// Normal Mapping Information
		Math::Vec3 Tangent;
	};

	class Mesh
	{
	private:
		unsigned int m_vertexCount;
	private:
		unsigned int m_vao, m_vbo, m_ebo;
	public:
		Mesh();
		Mesh(const std::vector<Vertex>& vertices, const std::vector<int>& indices);
		Mesh(const std::vector<NormalVertex>& vertices, const std::vector<int>& indices);
		Mesh(const Mesh& other);
		~Mesh();

		inline unsigned int GetVAO() const { return m_vao; }
		inline unsigned int GetVertexCount() const { return m_vertexCount; }
	private:
		void Initialize(const std::vector<Vertex>& vertices, const std::vector<int>& indices);
		void Initialize(const std::vector<NormalVertex>& vertices, const std::vector<int>& indices);
	};

	struct MeshCreationSpec
	{
		std::string filepath;
		void* mesh = NULL;
		const void* scene = NULL;

		std::map<std::string, BoneInfo>* map = NULL;
		int* counter = NULL;
		bool normalMapped = false;

		MeshCreationSpec(const std::string& filepath, void* mesh, const void* scene,
						 std::map<std::string, BoneInfo>* map, int* counter, bool normalMapped)
			: filepath(filepath), mesh(mesh), scene(scene), map(map), counter(counter), normalMapped(normalMapped)
		{
		}
	};
}
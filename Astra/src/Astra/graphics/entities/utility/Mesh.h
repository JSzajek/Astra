#pragma once

#include <string>
#include <vector>
#include <map>

#include "Astra/math/Maths.h"
#include "Astra/graphics/entities/utility/BoneInfo.h"
#include "Astra/graphics/materials/ImageMaterial.h"

namespace Astra::Graphics
{
	#define ASTRA_GL_TRIANGLES		0x0004
	#define ASTRA_TRIANGLE_STRIP	0x0005

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

		//Vertex() = default;

		//Vertex(float xPos, float yPos, float zPos,
		//	   float xNorm, float yNorm, float zNorm)
		//	: Position(xPos, yPos, zPos), TextureCoords(0), Normal(xNorm, yNorm, zNorm)
		//{
		//}

		//Vertex(float xPos, float yPos, float zPos, float xTex, float yTex, 
		//	   float xNorm, float yNorm, float zNorm)
		//	: Position(xPos, yPos, zPos), TextureCoords(xTex, yTex), Normal(xNorm, yNorm, zNorm)
		//{
		//}
	};

	struct NormalVertex : public Vertex
	{
		// Normal Mapping Information
		Math::Vec3 Tangent;

		/*NormalVertex() = default;

		NormalVertex(float xPos, float yPos, float zPos,
					 float xNorm, float yNorm, float zNorm,
					 float xTang, float yTang, float zTang)
			: Vertex(xPos, yPos, zPos, xNorm, yNorm, zNorm),
			  Tangent(xTang, yTang, zTang)
		{
		}

		NormalVertex(float xPos, float yPos, float zPos, float xTex, float yTex,
					 float xNorm, float yNorm, float zNorm,
			         float xTang, float yTang, float zTang)
			: Vertex(xPos, yPos, zPos, xTex, yTex, xNorm, yNorm, zNorm),
			  Tangent(xTang, yTang, zTang)
		{
		}*/
	};

	class Mesh
	{
	private:
		unsigned int m_vertexCount;
		unsigned int m_drawtype;
	private:
		unsigned int m_vao, m_vbo, m_ebo;
	public:
		Mesh();
		Mesh(unsigned int drawtype, const std::vector<float>* vertices, unsigned int dimensions);
		Mesh(const std::vector<Vertex>& vertices, const std::vector<int>& indices, unsigned int drawtype = ASTRA_GL_TRIANGLES);
		Mesh(const std::vector<NormalVertex>& vertices, const std::vector<int>& indices, unsigned int drawtype = ASTRA_GL_TRIANGLES);
		Mesh(const Mesh& other);
		~Mesh();

		inline unsigned int GetVAO() const { return m_vao; }
		inline unsigned int GetDrawType() const { return m_drawtype; }
		inline unsigned int GetVertexCount() const { return m_vertexCount; }
	private:
		void Initialize(const std::vector<float>* vertices, unsigned int dimensions);
		void Initialize(const std::vector<Vertex>& vertices, const std::vector<int>& indices);
		void Initialize(const std::vector<NormalVertex>& vertices, const std::vector<int>& indices);
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
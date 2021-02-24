#include "astra_pch.h"

#include "Terrain.h"
#include "Astra/graphics/ResourceManager.h"
#include "Astra/graphics/Resource.h"

#include <stb_image/stb_image.h>

namespace Astra::Graphics
{
	Terrain::Terrain()
		: Spatial(), material(), m_mesh(NULL), m_heights(NULL), m_vertexCount(0)
	{
	}

	Terrain::Terrain(int xGrid, int zGrid, const char* const heightmap, const TerrainMaterial& material)
		: Spatial(), material(material)
	{
		SetTranslation(Math::Vec3(xGrid * Size, 0, zGrid * Size));
		m_mesh = GeneratePlaneTerrain(heightmap);
	}

	Terrain::Terrain(int xGrid, int zGrid, float amplitude, int octaves, float roughness, const TerrainMaterial& material)
		: Spatial(), material(material)
	{
		SetTranslation(Math::Vec3(xGrid * Size, 0, zGrid * Size));
		HeightGenerator generator(amplitude, octaves, roughness);
		m_mesh = GeneratePlaneTerrain(&generator);
	}

	Terrain::Terrain(int xGrid, int zGrid, float amplitude, int octaves, float roughness, int seed, const TerrainMaterial& material)
		: Spatial(), material(material)
	{
		SetTranslation(Math::Vec3(xGrid * Size, 0, zGrid * Size));
		HeightGenerator generator(amplitude, octaves, roughness, seed);
		m_mesh = GeneratePlaneTerrain(&generator);
	}

	Terrain::Terrain(const Terrain& other)
		: Spatial(other), material(other.material),
			m_vertexCount(other.m_vertexCount), m_mesh(other.m_mesh)
	{
		m_heights = new float[m_vertexCount * m_vertexCount];
		memcpy(m_heights, other.m_heights, m_vertexCount * m_vertexCount * sizeof(float));
		TRACK(m_mesh);
	}
	
	void Terrain::operator=(const Terrain& other)
	{
		Name = other.Name;
		m_uid = other.m_uid;

		m_modelMatrix = new Math::Mat4(*other.m_modelMatrix);
		m_normalMatrix = new Math::Mat4(*other.m_normalMatrix);
		memcpy(m_data, other.m_data, sizeof(m_data));

		material = other.material;
		m_vertexCount = other.m_vertexCount;
		m_heights = new float[m_vertexCount * m_vertexCount];
		memcpy(m_heights, other.m_heights, m_vertexCount * m_vertexCount * sizeof(float));

		m_mesh = other.m_mesh;
		TRACK(m_mesh);
	}

	Terrain::~Terrain()
	{
		UNLOAD(m_mesh);
		delete m_heights;
	}

	Mesh* Terrain::GeneratePlaneTerrain(const char* const heightmap)
	{
		static int width, height;
		static unsigned char* buffer;

		stbi_set_flip_vertically_on_load(1);
		buffer = stbi_load(std::string(heightmap).c_str(), &width, &height, NULL, 1);

		m_vertexCount = height;
		m_heights = new float[m_vertexCount * m_vertexCount];

		int count = m_vertexCount * m_vertexCount;
		std::vector<int> indices;
		std::vector<Vertex> vertices;
		indices.reserve(6 * (m_vertexCount - 1) * (m_vertexCount - 1));
		vertices.reserve(count);
		for (int i = 0; i < m_vertexCount; i++)
		{
			for (int j = 0; j < m_vertexCount; j++)
			{
				Vertex vertex;
				m_heights[j + i * m_vertexCount] = GetHeight(j, i, buffer, height);
				vertex.Position = Math::Vec3(j / ((float)m_vertexCount - 1) * Size,
											 m_heights[j + i * m_vertexCount],
											 i / ((float)m_vertexCount - 1) * Size);
				vertex.Normal = CalculateNormal(j, i, buffer, height);
				vertex.TextureCoords = Math::Vec2(j / ((float)m_vertexCount - 1),
												  i / ((float)m_vertexCount - 1));
				vertices.push_back(vertex);
			}
		}
		for (int gz = 0; gz < m_vertexCount - 1; gz++)
		{
			for (int gx = 0; gx < m_vertexCount - 1; gx++)
			{
				int topLeft = (gz * m_vertexCount) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1) * m_vertexCount) + gx;
				int bottomRight = bottomLeft + 1;
				
				indices.push_back(topLeft);
				indices.push_back(bottomLeft);
				indices.push_back(topRight);
				indices.push_back(topRight);
				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
			}
		}
		stbi_image_free(buffer);

		return Resource::LoadMesh(heightmap, vertices, indices);
	}
	
	Mesh* Terrain::GeneratePlaneTerrain(HeightGenerator* const generator)
	{
		m_vertexCount = MAX_VERTICES;
		m_heights = new float[m_vertexCount * m_vertexCount];

		int count = m_vertexCount * m_vertexCount;
		std::vector<int> indices;
		std::vector<Vertex> vertices;
		indices.reserve(6 * (m_vertexCount - 1) * (m_vertexCount - 1));
		vertices.reserve(count);
		for (int i = 0; i < m_vertexCount; i++)
		{
			for (int j = 0; j < m_vertexCount; j++)
			{
				Vertex vertex;
				m_heights[j + i * m_vertexCount] = GetHeight(j, i, generator);
				vertex.Position = Math::Vec3(j / ((float)m_vertexCount - 1) * Size,
											 m_heights[j + i * m_vertexCount],
											 i / ((float)m_vertexCount - 1) * Size);
				vertex.Normal = CalculateNormal(j, i, generator);
				vertex.TextureCoords = Math::Vec2(j / ((float)m_vertexCount - 1),
												  i / ((float)m_vertexCount - 1));
				vertices.push_back(vertex);
			}
		}
		for (int gz = 0; gz < m_vertexCount - 1; gz++)
		{
			for (int gx = 0; gx < m_vertexCount - 1; gx++)
			{
				int topLeft = (gz * m_vertexCount) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1) * m_vertexCount) + gx;
				int bottomRight = bottomLeft + 1;

				indices.push_back(topLeft);
				indices.push_back(bottomLeft);
				indices.push_back(topRight);
				indices.push_back(topRight);
				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
			}
		}
		return Resource::LoadMesh(("HeightGenerator_" + std::to_string(generator->GetSeed())).c_str(), vertices, indices);
	}

	float Terrain::GetHeightOfTerrain(int xWorldCoord, int zWorldCoord)
	{
		int xTerrain = xWorldCoord - static_cast<int>(GetTranslation().x);
		int zTerrain = zWorldCoord - static_cast<int>(GetTranslation().z);

		float gridSquareSize = Size / (float)(m_vertexCount - 1);
		int xGrid = static_cast<int>(floorf(xTerrain / gridSquareSize));
		int zGrid = static_cast<int>(floorf(zTerrain / gridSquareSize));
		if (xGrid >= m_vertexCount - 1 || zGrid >= m_vertexCount - 1 || xGrid < 0 || zGrid < 0)
		{
			return 0;
		}
		float xCoord = (xTerrain % (int)gridSquareSize) / gridSquareSize;
		float zCoord = (zTerrain % (int)gridSquareSize) / gridSquareSize;
		
		if (xCoord <= (1 - zCoord))
		{
			return Math::BarryCentric(Math::Vec3(0, m_heights[xGrid + zGrid * m_vertexCount], 0),
									  Math::Vec3(1, m_heights[(xGrid + 1) + zGrid * m_vertexCount], 0),
									  Math::Vec3(0, m_heights[xGrid + (zGrid + 1) * m_vertexCount], 1),
									  Math::Vec2(xCoord, zCoord));
		}
		return Math::BarryCentric(Math::Vec3(1, m_heights[(xGrid + 1) + zGrid * m_vertexCount], 0),
								  Math::Vec3(1, m_heights[(xGrid + 1) + (zGrid + 1) * m_vertexCount], 1),
								  Math::Vec3(0, m_heights[xGrid + (zGrid + 1) * m_vertexCount], 1),
								  Math::Vec2(xCoord, zCoord));
	}

	float Terrain::GetHeight(int x, int z, const unsigned char* buffer, const int& imageHeight)
	{
		if (x < 0 || x >= imageHeight || z < 0 || z >= imageHeight)
		{
			return 0; // Out of bounds
		}
		return (2) * (buffer[imageHeight * z + x] /  255.0f) * MaxHeight;
	}

	float Terrain::GetHeight(int x, int z, HeightGenerator* const generator)
	{
		return generator->GenerateHeight(x, z);
	}
	
	Math::Vec3 Terrain::CalculateNormal(int x, int z, const unsigned char* buffer, const int& imageHeight)
	{
		float heightL = GetHeight(x - 1, z, buffer, imageHeight);
		float heightR = GetHeight(x + 1, z, buffer, imageHeight);
		float heightD = GetHeight(x, z - 1, buffer, imageHeight);
		float heightU = GetHeight(x, z + 1, buffer, imageHeight);
		Math::Vec3 result(heightL - heightR, 2.0f, heightD - heightU);
		result.Normalize();
		return result;
	}

	Math::Vec3 Terrain::CalculateNormal(int x, int z, HeightGenerator* const generator)
	{
		float heightL = GetHeight(x - 1, z, generator);
		float heightR = GetHeight(x + 1, z, generator);
		float heightD = GetHeight(x, z - 1, generator);
		float heightU = GetHeight(x, z + 1, generator);
		Math::Vec3 result(heightL - heightR, 2.0f, heightD - heightU);
		result.Normalize();
		return result;
	}
}
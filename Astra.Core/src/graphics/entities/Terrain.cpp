#include "Terrain.h"

namespace Astra::Graphics
{
	Terrain::Terrain(int xGrid, int zGrid, const TerrainMaterialPack* pack, const TerrainMaterial* map)
		: texturePack(pack), blendMap(map)
	{
		Translation().x = xGrid * Size;
		Translation().z = zGrid * Size;
		vertexArray = GeneratePlaneTerrain("res/textures/heightmap.png");
	}

	Terrain::~Terrain()
	{
		delete m_heights;
	}

	const VertexArray* Terrain::GeneratePlaneTerrain(const char* const heightmap)
	{
		static int width, height;
		static unsigned char* buffer;

		stbi_set_flip_vertically_on_load(1);
		buffer = stbi_load(std::string(heightmap).c_str(), &width, &height, NULL, 1);


		m_vertexCount = height;
		m_heights = new float[m_vertexCount * m_vertexCount];

		int count = m_vertexCount * m_vertexCount;
		std::vector<float> vertices(count * 3);
		std::vector<float> textureCoords(count * 2);
		std::vector<float> normals(count * 3);
		std::vector<int> indices(6 * (m_vertexCount - 1) * (m_vertexCount - 1));

		unsigned int pointer = 0;
		for (unsigned int i = 0; i < m_vertexCount; i++)
		{
			for (unsigned int j = 0; j < m_vertexCount; j++)
			{
				vertices[pointer * 3] = j / ((float)m_vertexCount - 1) * Size;
				m_heights[j + i * m_vertexCount] = GetHeight(j, i, buffer, height);
				vertices[(pointer * 3) + 1] = m_heights[j + i * m_vertexCount];
				vertices[(pointer * 3) + 2] = i / ((float)m_vertexCount - 1) * Size;
				const Math::Vec3& norm = CalculateNormal(j, i, buffer, height);
				normals[pointer * 3] = norm.x;
				normals[(pointer * 3) + 1] = norm.y;
				normals[(pointer * 3) + 2] = norm.z;
				textureCoords[pointer * 2] = j / ((float)m_vertexCount - 1);
				textureCoords[(pointer * 2) + 1] = i / ((float)m_vertexCount - 1);
				pointer++;
			}
		}
		pointer = 0;
		for (unsigned int gz = 0; gz < m_vertexCount - 1; gz++)
		{
			for (unsigned int gx = 0; gx < m_vertexCount - 1; gx++)
			{
				int topLeft = (gz * m_vertexCount) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1) * m_vertexCount) + gx;
				int bottomRight = bottomLeft + 1;
				indices[pointer++] = topLeft;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = topRight;
				indices[pointer++] = topRight;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = bottomRight;
			}
		}

		stbi_image_free(buffer);

		return Loader::Load(GL_TRIANGLES, vertices, indices, textureCoords, normals);
	}

	float Terrain::GetHeightOfTerrain(int xWorldCoord, int zWorldCoord)
	{
		int xTerrain = xWorldCoord - Translation().x;
		int zTerrain = zWorldCoord - Translation().z;

		float gridSquareSize = Size / (float)(m_vertexCount - 1);
		int xGrid = floorf(xTerrain / gridSquareSize);
		int zGrid = floorf(zTerrain / gridSquareSize);
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

	float Terrain::GetHeight(int x, int z, const unsigned char* buffer, const int& imageHeight)
	{
		if (x < 0 || x >= imageHeight || z < 0 || z >= imageHeight)
		{
			return 0; // Out of bounds
		}
		unsigned char pixel = buffer[imageHeight * z + x];
		if (pixel != 128)
		{
			int t = 1;
		}
		float height = (2) * (pixel /  255.0f);
		height *= MaxHeight;


		return height;
	}
}
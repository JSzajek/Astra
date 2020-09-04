#include "Terrain.h"

namespace Astra::Graphics
{
	Terrain::Terrain(int xGrid, int zGrid, Texture* texture)
		: texture(texture)
	{
		GetTranslation().x = xGrid * Size;
		GetTranslation().z = zGrid * Size;
		vertexArray = GeneratePlaneTerrain();
	}

	const VertexArray* Terrain::GeneratePlaneTerrain()
	{
		int count = VertexCount * VertexCount;
		std::vector<float> vertices(count * 3);
		std::vector<float> textureCoords(count * 2);
		std::vector<float> normals(count * 3);
		std::vector<int> indices(6 * (VertexCount - 1) * (VertexCount - 1));

		unsigned int pointer = 0;
		for (unsigned int i = 0; i < VertexCount; i++)
		{
			for (unsigned int j = 0; j < VertexCount; j++)
			{
				vertices[pointer * 3] = j / ((float)VertexCount - 1) * Size;
				vertices[(pointer * 3) + 2] = i / ((float)VertexCount - 1) * Size;
				normals[(pointer * 3) + 1] = 1;
				textureCoords[pointer * 2] = j / ((float)VertexCount - 1);
				textureCoords[(pointer * 2) + 1] = i / ((float)VertexCount - 1);
				pointer++;
			}
		}
		pointer = 0;
		for (unsigned int gz = 0; gz < VertexCount - 1; gz++)
		{
			for (unsigned int gx = 0; gx < VertexCount - 1; gx++)
			{
				int topLeft = (gz * VertexCount) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1) * VertexCount) + gx;
				int bottomRight = bottomLeft + 1;
				indices[pointer++] = topLeft;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = topRight;
				indices[pointer++] = topRight;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = bottomRight;
			}
		}
		return Loader::Load(GL_TRIANGLES, vertices, indices, textureCoords, normals);
	}
}
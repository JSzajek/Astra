#include "Terrain.h"

namespace Astra::Graphics
{
	Terrain::Terrain(unsigned int width, unsigned int height, Texture* texture)
	{}

	const VertexArray* Terrain::GeneratePlaneTerrain()
	{
		int count = VertexCount * VertexCount;
		float* vertices = new float[count * 3];
		


		delete[] vertices;
		return NULL;
	}
}
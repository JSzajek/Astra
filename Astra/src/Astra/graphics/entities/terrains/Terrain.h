#pragma once

#include "HeightGenerator.h"
#include "Astra/graphics/entities/Spatial.h"
#include "Astra/graphics/materials/TerrainMaterial.h"
#include "Astra/graphics/entities/utility/Mesh.h"

namespace Astra::Graphics
{
	#define MAX_VERTICES	128

	class Terrain : public Spatial
	{
	private:
		static constexpr float Size = 256;
		static constexpr float MaxHeight = 40;
	private:
		float* m_heights;
		int m_vertexCount;
		std::shared_ptr<Mesh> m_mesh;
	public:
		TerrainMaterial material;
	public:
		Terrain();
		Terrain(int xGrid, int zGrid, const char* const heightmap, const TerrainMaterial& material);
		Terrain(int xGrid, int zGrid, float amplitude, int octaves, float roughness, const TerrainMaterial& material);
		Terrain(int xGrid, int zGrid, float amplitude, int octaves, float roughness, int seed, const TerrainMaterial& material);
		Terrain(const Terrain& other);
		void operator=(const Terrain& other);
		~Terrain();
		
		inline std::shared_ptr<Mesh> GetMesh() const { return m_mesh; }
		float GetHeightOfTerrain(int xWorldCoord, int zWorldCoord);
	private:
		std::shared_ptr<Mesh> GeneratePlaneTerrain(const char* const heightmap);
		std::shared_ptr<Mesh> GeneratePlaneTerrain(HeightGenerator* const generator);
		float GetHeight(int x, int z, const unsigned char* buffer, const int& imageHeight);
		float GetHeight(int x, int z, HeightGenerator* const generator);
		Math::Vec3 CalculateNormal(int x, int z, const unsigned char* buffer, const int& imageHeight);
		Math::Vec3 CalculateNormal(int x, int z, HeightGenerator* const generator);
	};
}
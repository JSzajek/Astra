#include "HeightGenerator.h"

namespace Astra::Graphics
{
	// Implementation based on https://web.archive.org/web/20160530124230/http://freespace.virgin.net/hugo.elias/models/m_perlin.htm

	HeightGenerator::HeightGenerator(float amplitude, int octaves, float roughness)
		: m_seed(rand()), m_octaves(octaves), m_amplitude(amplitude), m_persistence(roughness)
	{
		Init(m_seed);
	}

	HeightGenerator::HeightGenerator(float amplitude, int octaves, float roughness, int seed)
		: m_seed(seed), m_octaves(octaves), m_amplitude(amplitude), m_persistence(roughness)
	{
		Init(m_seed);
	}

	HeightGenerator::~HeightGenerator()
	{
		delete[] m_primals;
	}

	void HeightGenerator::Init(int seed)
	{
		printf("Generating Terrain With Seed: %i\n", m_seed);

		m_seed = seed;

		srand(m_seed);
		m_primeIndex = rand() % MaxPrimals;
		m_primals = new int[MaxPrimals * 3];
		for (int i = 0; i < MaxPrimals; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				m_primals[j + i * 3] = rand();
			}
		}
	}

	float HeightGenerator::GenerateHeight(int x, int z)
	{
		float total = 0;
		float d = powf(2, m_octaves - 1);
		for (int i = 0; i < m_octaves; i++)
		{
			float freq = powf(2, i) / d;
			float ampl = powf(m_persistence, i) * m_amplitude;
			total += GetInterpolatedNoise(x * freq, z * freq, (m_primeIndex + i) % MaxPrimals) * ampl;
		}
		return total;
	}

	float HeightGenerator::GetNoise(int x, int z, int index) const
	{
		int n = x + z * 42;
		n = (n << 13) ^ n;
		int t = (n * (n * n * m_primals[index * 3] + m_primals[(index * 3) + 1]) + m_primals[(index * 3) + 2]) & 0x7fffffff;
		return 1.0 - (float)(t) / 1073741824.0f;
	}

	float HeightGenerator::GetSmoothNoise(int x, int z, int index) const
	{
		float corners = (GetNoise(x - 1, z - 1, index) + GetNoise(x + 1, z - 1, index) + GetNoise(x - 1, z + 1, index) + GetNoise(x + 1, z + 1, index)) / 16.0f;
		float sides = (GetNoise(x, z - 1, index) + GetNoise(x + 1, z, index) + GetNoise(x - 1, z, index) + GetNoise(x, z + 1, index)) / 8.0f;
		float center = GetNoise(x, z, index) / 4.0f;
		return center + sides + corners;
	}

	float HeightGenerator::GetInterpolatedNoise(float x, float z, int index) const
	{
		int xInt = static_cast<int>(x);
		int zInt = static_cast<int>(z);
		float xFrac = x - xInt;
		float zFrac = z - zInt;
		float v1 = GetSmoothNoise(xInt, zInt, index);
		float v2 = GetSmoothNoise(xInt + 1, zInt, index);
		float v3 = GetSmoothNoise(xInt, zInt + 1, index);
		float v4 = GetSmoothNoise(xInt + 1, zInt + 1, index);
		float i1 = CosineInterpolation(v1, v2, xFrac);
		float i2 = CosineInterpolation(v3, v4, xFrac);
		return CosineInterpolation(i1, i2, zFrac);
	}

	float HeightGenerator::CosineInterpolation(float a, float b, float blend) const
	{
		float theta = blend * M_PI;
		float f = (1.0f - cosf(theta)) * 0.5f;
		return a * (1.0f - f) + b * f;
	}
}
#pragma once

#include "../../../math/Maths.h"

namespace Astra::Graphics
{
	#define MaxPrimals		10
	
	class HeightGenerator
	{
	private:
		int m_seed;
		float m_amplitude;
		int m_octaves;
		float m_persistence;
		int* m_primals;
		int m_primeIndex;
	public:
		HeightGenerator(float amplitude, int octaves, float roughness);
		HeightGenerator(float amplitude, int octaves, float roughness, int seed);
		~HeightGenerator();
		float GenerateHeight(int x, int z);
		inline int GetSeed() const { return m_seed; }
	private:
		void Init(int seed);
		float GetNoise(int x, int z, int index) const;
		float GetSmoothNoise(int x, int z, int index) const;
		float GetInterpolatedNoise(float x, float z, int index) const;
		float CosineInterpolation(float a, float b, float blend) const;
	};
}
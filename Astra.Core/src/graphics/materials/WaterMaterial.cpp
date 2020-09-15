#include "WaterMaterial.h"

#include "../loaders/Loader.h"
#include "../Window.h"

namespace Astra::Graphics
{
	WaterMaterial::WaterMaterial()
		: waveStrength(DefaultWaveStrength), waveSpeed(DefaultWaveSpeed), shineDampener(DefaultShineDampener),
			reflectivity(DefaultReflectivity), baseColor(DefaultBaseColor), m_currentWaveSpeed(0),
				dudvTexture(Loader::LoadTexture(DefaultDuDvMap)), normalTexture(Loader::LoadTexture(DefaultNormalMap))
	{
	}

	WaterMaterial::WaterMaterial(const char* const dudvMapFilePath, const char* const normalMapFilePath)
		: waveStrength(DefaultWaveStrength), waveSpeed(DefaultWaveSpeed), shineDampener(DefaultShineDampener),
			reflectivity(DefaultReflectivity), baseColor(DefaultBaseColor), m_currentWaveSpeed(0),
			dudvTexture(Loader::LoadTexture(dudvMapFilePath)), normalTexture(Loader::LoadTexture(normalMapFilePath))
	{
	}

	float WaterMaterial::Increase()
	{
		m_currentWaveSpeed = fmod(m_currentWaveSpeed + waveSpeed * Window::delta, 1);
		return m_currentWaveSpeed;
	}
}
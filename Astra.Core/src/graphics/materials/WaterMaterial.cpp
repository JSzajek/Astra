#include "WaterMaterial.h"

#include "../loaders/Loader.h"
#include "../Window.h"

namespace Astra::Graphics
{
	WaterMaterial::WaterMaterial()
		: waveStrength(DefaultWaveStrength), waveSpeed(DefaultWaveSpeed), shineDampener(DefaultShineDampener),
			reflectivity(DefaultReflectivity), diffuseTexture(Loader::LoadTexture(DefaultDiffuseMap)), m_currentWaveSpeed(0),
				dudvTexture(Loader::LoadTexture(DefaultDuDvMap, false)), normalTexture(Loader::LoadTexture(DefaultNormalMap, false))
	{
		m_specular = Loader::LoadTexture(Texture::DefaultSpecular, false);
	}

	WaterMaterial::WaterMaterial(const char* const dudvMapFilePath, const char* const normalMapFilePath)
		: waveStrength(DefaultWaveStrength), waveSpeed(DefaultWaveSpeed), shineDampener(DefaultShineDampener),
			reflectivity(DefaultReflectivity), diffuseTexture(Loader::LoadTexture(DefaultDiffuseMap)), m_currentWaveSpeed(0),
			dudvTexture(Loader::LoadTexture(dudvMapFilePath, false)), normalTexture(Loader::LoadTexture(normalMapFilePath, false))
	{
		m_specular = Loader::LoadTexture(Texture::DefaultSpecular, false);
	}

	float WaterMaterial::Increase()
	{
		m_currentWaveSpeed = fmod(m_currentWaveSpeed + waveSpeed * Window::delta, 1);
		return m_currentWaveSpeed;
	}
}
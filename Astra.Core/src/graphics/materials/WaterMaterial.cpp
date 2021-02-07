#include "WaterMaterial.h"

#include "../loaders/Loader.h"
#include "../ResourceManager.h"

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

	float WaterMaterial::Increase(float delta)
	{
		m_currentWaveSpeed = fmodf(m_currentWaveSpeed + waveSpeed * delta, 1);
		return m_currentWaveSpeed;
	}

	WaterMaterial::~WaterMaterial()
	{
		ResourceManager::Unload(m_specular);
		ResourceManager::Unload(diffuseTexture);
		ResourceManager::Unload(dudvTexture);
		ResourceManager::Unload(normalTexture);
	}
}
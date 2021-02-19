#include "astra_pch.h"

#include "WaterMaterial.h"
#include "Astra/graphics/loaders/Loader.h"
#include "Astra/graphics/ResourceManager.h"

namespace Astra::Graphics
{
	const char* WaterMaterial::DefaultDiffuseMap	= RESOURCE("res/textures/DefaultWaterBase.jpg");
	const char* WaterMaterial::DefaultDuDvMap		= RESOURCE("res/textures/DefaultDuDvMap.png");
	const char* WaterMaterial::DefaultNormalMap		= RESOURCE("res/textures/DefaultNormalMap.png");

	WaterMaterial::WaterMaterial()
		: waveStrength(DefaultWaveStrength), waveSpeed(DefaultWaveSpeed), shineDampener(DefaultShineDampener),
			reflectivity(DefaultReflectivity), diffuseTexture(Loader::LoadTexture(DefaultDiffuseMap)), m_currentWaveSpeed(0),
				dudvTexture(Loader::LoadTexture(DefaultDuDvMap, false)), normalTexture(Loader::LoadTexture(DefaultNormalMap, false))
	{
		m_specular = Loader::LoadTexture(Texture::DefaultSpecular, false);
	}

	WaterMaterial::WaterMaterial(const char* const diffuseFilePath, const char* const dudvMapFilePath, const char* const normalMapFilePath)
		: waveStrength(DefaultWaveStrength), waveSpeed(DefaultWaveSpeed), shineDampener(DefaultShineDampener),
			reflectivity(DefaultReflectivity), diffuseTexture(Loader::LoadTexture(diffuseFilePath)), m_currentWaveSpeed(0),
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
		RESOURCE_UNLOAD(m_specular);
		RESOURCE_UNLOAD(diffuseTexture);
		RESOURCE_UNLOAD(dudvTexture);
		RESOURCE_UNLOAD(normalTexture);
	}

	void WaterMaterial::UpdateDiffuseMap(bool hdr)
	{
		Loader::UpdateDiffuseTexture(diffuseTexture, hdr);
	}
}
#include "astra_pch.h"

#include "WaterMaterial.h"
#include "Astra/graphics/Resource.h"

namespace Astra::Graphics
{
	const char* WaterMaterial::DefaultDiffuseMap	= RESOURCE("res/textures/DefaultWaterBase.jpg");
	const char* WaterMaterial::DefaultDuDvMap		= RESOURCE("res/textures/DefaultDuDvMap.png");
	const char* WaterMaterial::DefaultNormalMap		= RESOURCE("res/textures/DefaultNormalMap.png");

	WaterMaterial::WaterMaterial()
		: waveStrength(DefaultWaveStrength), waveSpeed(DefaultWaveSpeed), shineDampener(DefaultShineDampener),
			reflectivity(DefaultReflectivity), diffuseTexture(Resource::LoadTexture(DefaultDiffuseMap, true)), m_currentWaveSpeed(0),
				dudvTexture(Resource::LoadTexture(DefaultDuDvMap)), normalTexture(Resource::LoadTexture(DefaultNormalMap))
	{
		m_specular = Resource::LoadTexture(Texture::DefaultSpecular, false);
	}

	WaterMaterial::WaterMaterial(const char* const diffuseFilePath, const char* const dudvMapFilePath, const char* const normalMapFilePath)
		: waveStrength(DefaultWaveStrength), waveSpeed(DefaultWaveSpeed), shineDampener(DefaultShineDampener),
			reflectivity(DefaultReflectivity), diffuseTexture(Resource::LoadTexture(diffuseFilePath)), m_currentWaveSpeed(0),
			dudvTexture(Resource::LoadTexture(dudvMapFilePath, false)), normalTexture(Resource::LoadTexture(normalMapFilePath, false))
	{
		m_specular = Resource::LoadTexture(Texture::DefaultSpecular, false);
	}

	WaterMaterial::WaterMaterial(const WaterMaterial& other)
		: waveStrength(other.waveStrength), waveSpeed(other.waveSpeed), shineDampener(other.shineDampener),
		reflectivity(other.reflectivity), diffuseTexture(other.diffuseTexture), m_currentWaveSpeed(other.m_currentWaveSpeed),
		dudvTexture(other.dudvTexture), normalTexture(other.normalTexture), m_specular(other.m_specular)
	{
		TRACK(m_specular);
		TRACK(diffuseTexture);
		TRACK(dudvTexture);
		TRACK(normalTexture);
	}
	
	void WaterMaterial::operator=(const WaterMaterial& other)
	{
		waveStrength = other.waveStrength;
		waveSpeed = other.waveSpeed;
		shineDampener = other.shineDampener;
		reflectivity = other.reflectivity;
		diffuseTexture = other.diffuseTexture;
		m_currentWaveSpeed = other.m_currentWaveSpeed;
		dudvTexture = other.dudvTexture;
		normalTexture = other.normalTexture; 
		m_specular = other.m_specular;

		TRACK(m_specular);
		TRACK(diffuseTexture);
		TRACK(dudvTexture);
		TRACK(normalTexture);
	}

	WaterMaterial::~WaterMaterial()
	{
		UNLOAD(m_specular);
		UNLOAD(diffuseTexture);
		UNLOAD(dudvTexture);
		UNLOAD(normalTexture);
	}
	
	float WaterMaterial::Increase(float delta)
	{
		m_currentWaveSpeed = fmodf(m_currentWaveSpeed + waveSpeed * delta, 1);
		return m_currentWaveSpeed;
	}
}
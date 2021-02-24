#include "astra_pch.h"

#include "TerrainMaterial.h"
#include "Astra/graphics/loaders/Loader.h"
#include "Astra/graphics/ResourceManager.h"
#include "Astra/graphics/Resource.h"

namespace Astra::Graphics
{
	TerrainMaterial::TerrainMaterial()
		: m_blendMapTexture(NULL), m_backgroundTexture(NULL), m_rTexture(NULL),
		  m_gTexture(NULL), m_bTexture(NULL), specularTexture(NULL), 
		  reflectivity(0), shineDampener(1)
	{
	}

	TerrainMaterial::TerrainMaterial(const char* const blendMap, const char* const background, 
									 const char* const red, const char* const green, const char* const blue)
		: m_blendMapTexture(Resource::LoadTexture(blendMap)), m_backgroundTexture(Resource::LoadTexture(background, true)), m_rTexture(Resource::LoadTexture(red, true)),
		  m_gTexture(Resource::LoadTexture(green, true)), m_bTexture(Resource::LoadTexture(blue, true)), specularTexture(Resource::LoadTexture(Texture::DefaultSpecular)),
		  reflectivity(0), shineDampener(1)
	{
	}


	TerrainMaterial::TerrainMaterial(const TerrainMaterial& other)
		: m_blendMapTexture(other.m_blendMapTexture), m_backgroundTexture(other.m_backgroundTexture), m_rTexture(other.m_rTexture),
		  m_gTexture(other.m_gTexture), m_bTexture(other.m_bTexture), specularTexture(other.specularTexture),
		  reflectivity(other.reflectivity), shineDampener(other.shineDampener)
	{
		TRACK(m_blendMapTexture);
		TRACK(m_backgroundTexture);
		TRACK(m_rTexture);
		TRACK(m_gTexture);
		TRACK(m_bTexture);
		TRACK(specularTexture);
	}

	void TerrainMaterial::operator=(const TerrainMaterial& other)
	{
		m_blendMapTexture = other.m_blendMapTexture;
		m_backgroundTexture = other.m_backgroundTexture;
		m_rTexture = other.m_rTexture;
		m_gTexture = other.m_gTexture;
		m_bTexture = other.m_bTexture;
		specularTexture = other.specularTexture;
		reflectivity = other.reflectivity;
		shineDampener = other.shineDampener;

		TRACK(m_blendMapTexture);
		TRACK(m_backgroundTexture);
		TRACK(m_rTexture);
		TRACK(m_gTexture);
		TRACK(m_bTexture);
		TRACK(specularTexture);
	}

	TerrainMaterial::~TerrainMaterial()
	{
		UNLOAD(m_blendMapTexture);
		UNLOAD(m_backgroundTexture);
		UNLOAD(m_rTexture);
		UNLOAD(m_gTexture);
		UNLOAD(m_bTexture);
		UNLOAD(specularTexture);
	}
}

#include "astra_pch.h"

#include "Astra/Log.h"
#include "SkyboxMaterial.h"
#include "Astra/graphics/loaders/Loader.h"
#include "Astra/graphics/ResourceManager.h"

namespace Astra::Graphics
{
	SkyboxMaterial::SkyboxMaterial(const std::vector<const char*>& firstFiles)
		: m_secondaryTexture(NULL), m_blendFactor(0)
	{
		m_primaryTexture = Loader::LoadCubeMap(firstFiles);
	}
	
	SkyboxMaterial::SkyboxMaterial(const std::vector<const char*>& firstFiles, const std::vector<const char*>& secondFiles)
		: m_blendFactor(0)
	{
		m_primaryTexture = Loader::LoadCubeMap(firstFiles);
		m_secondaryTexture = Loader::LoadCubeMap(secondFiles);
	}

	SkyboxMaterial::~SkyboxMaterial()
	{
		RESOURCE_UNLOAD(m_primaryTexture);
		RESOURCE_UNLOAD(m_secondaryTexture);
	}

	const unsigned int SkyboxMaterial::GetFirstTextureId() const
	{
		if (m_primaryTexture != NULL)
		{
			return m_primaryTexture->id;
		}
		ASTRA_CORE_WARN("SkyboxMaterial: Skybox Primary Texture Not Set.");
		return 0;
	}

	const unsigned int SkyboxMaterial::GetSecondTextureId() const
	{
		if (m_secondaryTexture != NULL)
		{
			return m_secondaryTexture->id;
		}
		ASTRA_CORE_WARN("SkyboxMaterial: Skybox Secondary Texture Not Set.");
		return m_primaryTexture->id;
	}

	void SkyboxMaterial::UpdateDiffuseMap(bool hdr)
	{
		Loader::UpdateCubeMap(m_primaryTexture, hdr);
		if (m_secondaryTexture)
		{
			Loader::UpdateCubeMap(m_secondaryTexture, hdr);
		}
	}
}
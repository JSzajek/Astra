#include "SkyboxMaterial.h"

#include "../../logger/Logger.h"
#include "../loaders/Loader.h"

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

	const unsigned int& SkyboxMaterial::GetFirstTextureId() const
	{
		if (m_primaryTexture != NULL)
		{
			return m_primaryTexture->id;
		}
		Logger::LogWarning("Skybox Primary Texture Not Set.");
		return 0;
	}

	const unsigned int& SkyboxMaterial::GetSecondTextureId() const
	{
		if (m_secondaryTexture != NULL)
		{
			return m_secondaryTexture->id;
		}
		Logger::LogWarning("Skybox Secondary Texture Not Set.");
		return m_primaryTexture->id;
	}
}
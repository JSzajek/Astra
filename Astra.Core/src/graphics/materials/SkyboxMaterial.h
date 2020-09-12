#pragma once

#include <vector>

#include "../buffers/Texture.h"
#include "../buffers/CubeMapTexture.h"

namespace Astra::Graphics
{
	class SkyboxMaterial
	{
	private:
		const CubeMapTexture* m_primaryTexture;
		const CubeMapTexture* m_secondaryTexture;
		float m_blendFactor;
	public:
		SkyboxMaterial(const std::vector<const char*>& firstFiles);
		SkyboxMaterial(const std::vector<const char*>& firstFiles, const std::vector<const char*>& secondFiles);
		
		const unsigned int& GetFirstTextureId() const;
		const unsigned int& GetSecondTextureId() const;
		inline const float& GetBlendFactor() const { return m_blendFactor; }
		inline float& BlendFactor() { return m_blendFactor; }
	};
}
#pragma once

#include <vector>

#include "Astra/graphics/buffers/Texture.h"
#include "Astra/graphics/buffers/CubeMapTexture.h"

namespace Astra::Graphics
{
	class SkyboxMaterial
	{
	private:
		CubeMapTexture* m_primaryTexture;
		CubeMapTexture* m_secondaryTexture;
		float m_blendFactor;
	public:
		SkyboxMaterial(const std::vector<const char*>& firstFiles);
		SkyboxMaterial(const std::vector<const char*>& firstFiles, const std::vector<const char*>& secondFiles);
		~SkyboxMaterial();

		const unsigned int GetFirstTextureId() const;
		const unsigned int GetSecondTextureId() const;
		inline const float& GetBlendFactor() const { return m_blendFactor; }
		inline float& BlendFactor() { return m_blendFactor; }
	public:
		void UpdateDiffuseMap(bool hdr);
	};
}
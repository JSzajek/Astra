#pragma once

#include "Astra/graphics/buffers/Texture.h"
#include "Astra/Core.h"

namespace Astra::Graphics
{
	class ImageMaterial
	{
	private:
		// Data
		Texture* m_textures[5] = {};
		unsigned char m_flags;
	private:
		// Properties
		float m_height;
		float m_reflectivity;
		bool m_transparent;
		bool m_fakeLight;
	public:		
		ImageMaterial();
		ImageMaterial(const std::vector<Texture*>& textures);
		ImageMaterial(const ImageMaterial& other);
		void operator=(const ImageMaterial& other);
		~ImageMaterial();

		inline unsigned int GetTextureId(TextureType type) const 
		{
			ASTRA_CORE_ASSERT(HasTexture(type), "ImageMaterial: Attempted to Retrieve Uninitialized Texture");
			return m_textures[type]->id;
		}
		inline bool HasTexture(TextureType type) const { return m_flags & BIT(static_cast<unsigned char>(type)); }

		inline float GetHeightOffset() const { return m_height; }
		inline float GetReflectivity() const { return m_reflectivity; }
		inline bool GetTransparency() const { return m_transparent; }
		inline bool GetFakeLighting() const { return m_fakeLight; }

		inline void SetHeightOffset(float value) { m_height = value; }
		inline void SetReflectivity(float value) { m_reflectivity = value; }
		inline void SetTransparency(bool enabled) { m_transparent = enabled; }
		inline void SetFakeLighting(bool enabled) { m_fakeLight = enabled; }
		
		void AddTexture(Texture* texture);
	};
}
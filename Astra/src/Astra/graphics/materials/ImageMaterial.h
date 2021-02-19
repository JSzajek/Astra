#pragma once

#include "../buffers/Texture.h"
#include "../../math/Vec3.h"

namespace Astra::Graphics
{
	class ImageMaterial
	{
	private:
		size_t m_rowCount;

		Texture* m_diffuse;
		const Texture* m_specular;
		const Texture* m_emission;

		const Texture* m_normalMap;
		const Texture* m_parallaxMap;
		float m_height;
	public:
		float Reflectivity;
		bool Transparent;
		bool FakeLight;
	public:		
		ImageMaterial(const char* diffuse,
					  const char* specular,
					  const char* emission,
					  size_t rowCount, float reflectivity,
					  bool transparent);

		ImageMaterial(const char* diffuse, 
					  const char* specular, 
					  const char* normalMap, 
					  const char* parallaxMap,
					  float heightOffset,
					  const char* emission, 
					  size_t rowCount, 
					  float reflectivity, 
					  bool transparent);

		~ImageMaterial();

		inline bool IsNormalMapped() const { return m_normalMap != NULL; }
		inline bool IsParallaxMapped() const { return m_parallaxMap != NULL; }
		inline bool HasGlow() const { return m_emission != NULL; }

		inline size_t GetRowCount() const { return m_rowCount; }
		inline unsigned int GetId() const { return m_diffuse->id; }
		inline unsigned int GetSpecularId() const { return m_specular->id; }
		inline unsigned int GetEmissionId() const { return m_emission == NULL ? 0 :m_emission->id; }
		
		inline unsigned int GetNormalMapId() const { return m_normalMap->id; }
		inline unsigned int GetParallaxMapId() const { return m_parallaxMap->id; }
		inline float GetHeightOffset() const { return m_height; }
	public:
		void UpdateDiffuseMap(bool hdr);
	};
}
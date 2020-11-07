#pragma once

#include "../buffers/Texture.h"
#include "../../math/Vec3.h"

namespace Astra::Graphics
{
	class ImageMaterial
	{
	private:
		int m_rowCount;
		Texture m_diffuse;
		Texture m_specular;
		Texture m_emission;
		bool m_glowing;
	public:
		float Reflectivity;
		bool Transparent;
		bool FakeLight;
	public:
		ImageMaterial(const char* const filepath = Texture::DefaultTexture, 
					  int rowCount = 1, float reflectivity = 0.25f,
					  bool transparent = false, bool fakeLight = false);

		ImageMaterial(const char* const diffusepath,
					  const char* const specularpath = NULL,
					  int rowCount = 1, float reflectivity = 0.25f,
			          bool transparent = false);

		ImageMaterial(const char* const diffusepath,
					  const char* const specularpath,
					  const char* const emmissionpath,
					  int rowCount = 1, float reflectivity = 0.25f,
					  bool transparent = false);

		//ImageMaterial(const char* const filepath,
		//			  const char* const specularpath,
		//			  const char* const heightmappath = NULL,
		//			  int rowCount = 1, float reflectivity = 0.25f,
		//			  bool transparent = false);

		inline bool HasGlow() const { return m_glowing; }

		inline int GetRowCount() const { return m_rowCount; }
		inline unsigned int GetId() const { return m_diffuse.id; }
		inline unsigned int GetSpecularId() const { return m_specular.id; }
		inline unsigned int GetEmissionId() const { return m_emission.id; }
	};
}
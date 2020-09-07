#pragma once

#include "../buffers/Texture.h"

namespace Astra::Graphics
{
	class ImageMaterial : public Texture
	{
	private:
		int m_rowCount;
	public:
		float shineDampener, reflectivity;
		bool transparent;
		bool fakeLight;
	public:
		ImageMaterial();
		ImageMaterial(const char* const filepath);
		ImageMaterial(const char* const filepath, int rowCount);
		ImageMaterial(const char* const filepath, int rowCount = 1, float shineDampener = 1, float reflectivity = 0, bool transparent = false, bool fakeLight = false);
		
		inline int GetRowCount() const { return m_rowCount; }
	};
}
#pragma once

#include "../buffers/Texture.h"

namespace Astra::Graphics
{
	class ImageMaterial : public Texture
	{
	public:
		float shineDampener, reflectivity;
		bool transparent;
		bool fakeLight;
	public:
		ImageMaterial(const char* const filepath);
		ImageMaterial(const char* const filepath, float shineDampener, float reflectivity, bool transparent = false, bool fakeLight = false);
	};
}
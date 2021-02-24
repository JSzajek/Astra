#pragma once

#include <vector>

#include "Texture.h"
#include "Astra/Res.h"

namespace Astra::Graphics
{
	enum CubeMapFace
	{
		Right	= 0,
		Left	= 1,
		Top		= 2,
		Bottom	= 3,
		Back	= 4,
		Front	= 5
	};

	class CubeMapTexture : public Res
	{
	private:
		std::vector<const char*> m_filepaths;
	public:
		int width, height; // Used in gui textures - TODO
		unsigned int id;
		bool hdr;
		TextureType type = TextureType::None;
	public:
		CubeMapTexture();
		CubeMapTexture(const std::vector<const char*>& filepaths);
		inline const std::vector<const char*>& GetFiles() const { return m_filepaths; }
		virtual void Free() override;
	};
}
#pragma once

#include <vector>
#include "Texture.h"

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

	class CubeMapTexture
	{
	private:
		std::vector<const char*> m_filepaths;
		Texture** m_faces;
	public:
		unsigned int id;
		bool hdr;
	public:
		CubeMapTexture(const std::vector<const char*>& filepaths);
		~CubeMapTexture();

		Texture* operator[](int index); 

		inline const std::vector<const char*>& GetFiles() const { return m_filepaths; }
	};
}
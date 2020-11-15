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
		Texture m_faces[6];
	public:
		GLuint id;
	public:
		CubeMapTexture(const std::vector<const char*>& filepaths)
			: m_filepaths(filepaths), id(0)
		{
			for (size_t i = 0; i < filepaths.size(); i++)
			{
				m_faces[i].m_filePath = filepaths[i];
			}
		}

		Texture& operator[](int index) { return m_faces[index]; }
	};
}
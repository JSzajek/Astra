#include "astra_pch.h"

#include "CubeMapTexture.h"
#include "../ResourceManager.h"

namespace Astra::Graphics
{
	CubeMapTexture::CubeMapTexture(const std::vector<const char*>& filepaths)
		: m_filepaths(filepaths), id(0)
	{
		m_faces = new Texture*[6];
		for (size_t i = 0; i < filepaths.size(); i++)
		{
			m_faces[i] = new Texture(filepaths[i]);
		}
	}

	CubeMapTexture::~CubeMapTexture()
	{
		for (size_t i = 0; i < m_filepaths.size(); i++)
		{
			ResourceManager::Unload(m_faces[i]);
		}
		delete[] m_faces;
	}

	Texture* CubeMapTexture::operator[](int index)
	{
		return m_faces[index];
	}
}
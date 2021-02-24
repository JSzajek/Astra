#include "astra_pch.h"

#include "GuiMaterial.h"
#include "Astra/graphics/Resource.h"

namespace Astra::Graphics
{
	GuiMaterial::GuiMaterial()
		: m_texture(NULL), m_rowCount(1)
	{
	}

	GuiMaterial::GuiMaterial(const char* const filepath, int rowCount)
		: m_texture(Resource::LoadTexture(filepath, false)), m_rowCount(rowCount)
	{
		m_size = Math::Vec2(static_cast<float>(m_texture->width / m_rowCount), static_cast<float>(m_texture->height / m_rowCount));
	}

	GuiMaterial::GuiMaterial(const GuiMaterial& other)
		: m_texture(other.m_texture), m_rowCount(other.m_rowCount), m_size(other.m_size)
	{
		TRACK(m_texture);
	}

	void GuiMaterial::operator=(const GuiMaterial& other)
	{
		m_texture = other.m_texture;
		m_rowCount = other.m_rowCount;
		m_size = other.m_size;

		TRACK(m_texture);
	}

	GuiMaterial::~GuiMaterial()
	{
		UNLOAD(m_texture);
	}
}
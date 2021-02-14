#include "astra_pch.h"

#include "WaterTile.h"
#include "../../ResourceManager.h"

namespace Astra::Graphics
{
	WaterTile::WaterTile(float xCenter, float zCenter, float height, WaterMaterial* const material)
		: Spatial(), m_size(DefaultSize)
	{
		m_rows[0].x = xCenter;
		m_rows[0].y = height;
		m_rows[0].z = zCenter;
		m_rows[2] = m_size;
		UpdateMatrices();
		this->material = ResourceManager::Track(material);
	}

	WaterTile::WaterTile(float xCenter, float zCenter, float height, float size, WaterMaterial* const material)
		: m_size(size)
	{
		m_rows[0].x = xCenter;
		m_rows[0].y = height;
		m_rows[0].z = zCenter;
		m_rows[2] = m_size;
		UpdateMatrices();
		this->material = ResourceManager::Track(material);
	}

	WaterTile::WaterTile(const WaterTile& other)
		: m_size(other.m_size), material(other.material)
	{
		memcpy(m_data, other.m_data, 3 * 3 * sizeof(float));
	}

	WaterTile::~WaterTile()
	{
		ResourceManager::Unload(material);
	}
}
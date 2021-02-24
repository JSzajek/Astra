#include "astra_pch.h"

#include "WaterTile.h"
#include "Astra/graphics/ResourceManager.h"

namespace Astra::Graphics
{
	WaterTile::WaterTile()
		: m_size(0), material()
	{
	}

	WaterTile::WaterTile(float xCenter, float zCenter, float height, float size)
		: m_size(size), material()
	{
		m_rows[0].x = xCenter;
		m_rows[0].y = height;
		m_rows[0].z = zCenter;
		m_rows[2] = m_size;
		UpdateMatrices();
	}

	WaterTile::WaterTile(const WaterMaterial& material, float xCenter, float zCenter, float height, float size)
		: material(material)
	{
		m_rows[0].x = xCenter;
		m_rows[0].y = height;
		m_rows[0].z = zCenter;
		m_rows[2] = m_size;
		UpdateMatrices();
	}

	WaterTile::WaterTile(const WaterTile& other)
		: m_size(other.m_size), material(other.material)
	{
		memcpy(m_data, other.m_data, 3 * 3 * sizeof(float));
	}
}
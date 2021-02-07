#include "WaterTile.h"

#include "../../../logger/Logger.h"
#include "../../ResourceManager.h"

namespace Astra::Graphics
{
	WaterTile::WaterTile(float xCenter, float zCenter, float height, WaterMaterial* const material)
		: Spatial(), m_size(DefaultSize)
	{
		rows[0].x = xCenter;
		rows[0].y = height;
		rows[0].z = zCenter;
		rows[2] = m_size;
		UpdateMatrices();
		this->material = ResourceManager::Track(material);
	}

	WaterTile::WaterTile(float xCenter, float zCenter, float height, float size, WaterMaterial* const material)
		: m_size(size)
	{
		rows[0].x = xCenter;
		rows[0].y = height;
		rows[0].z = zCenter;
		rows[2] = m_size;
		UpdateMatrices();
		this->material = ResourceManager::Track(material);
	}

	WaterTile::WaterTile(const WaterTile& other)
		: m_size(other.m_size), material(other.material)
	{
		memcpy(data, other.data, 3 * 3 * sizeof(float));
	}

	WaterTile::~WaterTile()
	{
		ResourceManager::Unload(material);
	}

	Math::Vec3* WaterTile::Rotation()
	{
		Logger::LogWarning("Accessing Deleted Function.");
		return NULL;
	}
}
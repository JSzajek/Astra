#include "WaterTile.h"

#include "../../logger/Logger.h"

namespace Astra::Graphics
{
	WaterTile::WaterTile(float xCenter, float zCenter, float height, WaterMaterial* const material)
		: m_size(DefaultSize), material(material)
	{
		rows[0].x = xCenter;
		rows[0].y = height;
		rows[0].z = zCenter;
		rows[2] = m_size;
	}

	WaterTile::WaterTile(float xCenter, float zCenter, float height, float size, WaterMaterial* const material)
		: m_size(size), material(material)
	{
		rows[0].x = xCenter;
		rows[0].y = height;
		rows[0].z = zCenter;
		rows[2] = m_size;
	}

	WaterTile::WaterTile(const WaterTile& other)
		: m_size(other.m_size), material(other.material)
	{
		memcpy(data, other.data, 3 * 3 * sizeof(float));
	}


	Math::Vec3& WaterTile::Rotation()
	{
		static Math::Vec3 empty;
		Logger::LogWarning("Accessing Deleted Function.");
		return empty;
	}
}
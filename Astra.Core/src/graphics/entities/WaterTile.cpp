#include "WaterTile.h"

#include "../../logger/Logger.h"

namespace Astra::Graphics
{
	WaterTile::WaterTile(float xCenter, float zCenter, float height)
		: m_size(DefaultSize)
	{
		rows[0].x = xCenter;
		rows[0].y = height;
		rows[0].z = zCenter;
		rows[2] = m_size;
	}

	WaterTile::WaterTile(float xCenter, float zCenter, float height, float size)
		: m_size(size)
	{
		rows[0].x = xCenter;
		rows[0].y = height;
		rows[0].z = zCenter;
		rows[2] = m_size;
	}

	WaterTile::WaterTile(const WaterTile& other)
		: m_size(other.m_size)
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
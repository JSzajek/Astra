#include "astra_pch.h"

#include "Rect2.h"

namespace Astra::Graphics
{
	Rect2::Rect2()
		: m_position(0), m_size(0)
	{
	}

	Rect2::Rect2(float x, float y, float width, float height)
		: m_position(static_cast<int>(x), static_cast<int>(y)), m_size(static_cast<int>(width), static_cast<int>(height))
	{
	}

	Rect2::Rect2(int x, int y, int width, int height)
		: m_position(x, y), m_size(width, height)
	{
	}

	Rect2::Rect2(const Rect2& other)
		: m_position(other.m_position), m_size(other.m_size)
	{
	}

	bool Rect2::HasPoint(const Math::Vec2& point) const
	{
		return point.x >= m_position.x && point.y >= m_position.y
			&& point.x < (m_position.x + m_size.x) && point.y < (m_position.y + m_size.y);
	}
}
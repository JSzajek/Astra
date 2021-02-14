#pragma once

#include "../../../math/iVec2.h"
#include "../../../math/Vec2.h"

namespace Astra::Graphics
{
	class Rect2
	{
		Math::iVec2 m_position;
		Math::iVec2 m_size;
	public:
		Rect2();
		Rect2(float x, float y, float width, float height);
		Rect2(int x, int y, int width, int height);
		Rect2(const Rect2& other);

		bool HasPoint(const Math::Vec2& point) const;

		inline Math::iVec2& GetPosition() { return m_position; }
		inline Math::iVec2& GetSize() { return m_size; }

		inline void SetPosition(const Math::iVec2& position) { m_position = position; }
		inline void SetSize(const Math::iVec2& size) { m_size = size; }
	};
}
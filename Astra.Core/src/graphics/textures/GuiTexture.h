#pragma once

#include "../../math/Vec2.h"

namespace Astra::Graphics
{
	class GuiTexture
	{
	public:
		std::string Name;
	private:
		Math::Vec2 m_position;
		Math::Vec2 m_scale;
		unsigned int m_id;
		int m_offset;
	public:
		GuiTexture(const char* name, const Math::Vec2& position, const Math::Vec2& scale);
		GuiTexture(const char* name, const unsigned int& id, const Math::Vec2& position, const Math::Vec2& scale);
	
		inline void SetOffset(int offset) { m_offset = offset; }

		inline Math::Vec2 GetPosition() const { return m_position; }
		inline Math::Vec2 GetScale() const { return m_scale; }
		inline unsigned int GetId() const { return m_id; }
		inline int GetOffset() const { return m_offset; }
	};
}
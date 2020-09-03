#pragma once

#include <GL/glew.h>
#include "../../math/Vec2.h"

namespace Astra::Graphics
{
	class GuiTexture
	{
	private:
		Math::Vec2 m_position;
		Math::Vec2 m_scale;
		GLuint m_id;
	public:
		GuiTexture(const Math::Vec2& position, const Math::Vec2& scale);
		GuiTexture(const GLuint& id, const Math::Vec2& position, const Math::Vec2& scale);
	
		void SetId(const GLuint& id) { m_id = id; }

		inline Math::Vec2 GetPosition() const { return m_position; }
		inline Math::Vec2 GetScale() const { return m_scale; }
		inline GLuint GetId() const { return m_id; }
	};
}
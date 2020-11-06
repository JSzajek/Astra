#pragma once

#include <GL/glew.h>
#include <cstring>

namespace Astra::Graphics
{
	enum class DepthBufferType
	{
		Render,
		Texture,
		None
	};

	struct FrameBuffer
	{
	private:
		GLuint m_data[3];
	public:
		FrameBuffer()
		{
			memset(m_data, 0, 3 * sizeof(GLuint));
		}

		const GLuint& GetId() const { return m_data[0]; }
		const GLuint& GetColorAttachment() const { return m_data[1]; }
		const GLuint& GetDepthAttachment() const { return m_data[2]; }

		GLuint& Id() { return m_data[0]; }
		GLuint& ColorAttachment() { return m_data[1]; }
		GLuint& DepthAttachment() { return m_data[2]; }
	};
}
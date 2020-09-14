#pragma once

#include <GL/glew.h>
#include <cstring>

namespace Astra::Graphics
{
	struct FrameBuffer
	{
	private:
		GLuint m_data[3];
	public:
		FrameBuffer()
		{
			memset(m_data, 0, 3 * sizeof(float));
		}

		/*FrameBuffer(const FrameBuffer& other)
		{
			memcpy(m_data, other.m_data, 3 * sizeof(float));
		}*/

		const GLuint& GetId() const { return m_data[0]; }
		const GLuint& GetColorAttachment() const { return m_data[1]; }
		const GLuint& GetDepthAttachment() const { return m_data[2]; }

		GLuint& Id() { return m_data[0]; }
		GLuint& ColorAttachment() { return m_data[1]; }
		GLuint& DepthAttachment() { return m_data[2]; }
	};
}
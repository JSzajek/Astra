#pragma once

#include "../shaders/Shader.h"
#include "../buffers/FrameBuffer.h"

namespace Astra::Graphics
{
	class ImageEffect
	{
	private:
		int m_width, m_height;
		const FrameBuffer* m_buffer;
	protected:
		Shader* m_shader;
	public:
		ImageEffect(Shader* shader);
		ImageEffect(Shader* shader, int width, int height, bool floating = false, unsigned int clamping = GL_LINEAR);
		~ImageEffect();
		void Start(unsigned int* attachment) const;
		void Stop() const;
	private:
		void BindBuffer() const;
		void UnbindBuffer() const;
	};
}
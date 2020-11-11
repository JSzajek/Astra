#pragma once

#include "../shaders/Shader.h"
#include "../buffers/FrameBuffer.h"

namespace Astra::Graphics
{
	class ImageEffect
	{
	private:
		size_t m_totalSteps;
	protected:
		size_t m_step;
		int m_width, m_height;
		const FrameBuffer* m_buffer;
		Shader* m_shader;
	public:
		ImageEffect(Shader* shader, size_t totalSteps = 1);
		ImageEffect(Shader* shader, int width, int height, size_t totalSteps = 1, bool floating = false, unsigned int clamping = GL_LINEAR);
		~ImageEffect();
		virtual void Start(unsigned int* attachment);
		virtual void Stop();
		inline bool Finished() { return m_totalSteps == m_step; }
		inline void Reset() { m_step = 0; }
	protected:
		void BindBuffer(unsigned int id) const;
		void UnbindBuffer() const;
	};
}
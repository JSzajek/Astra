#pragma once

#include "../shaders/Shader.h"
#include "../buffers/FrameBuffer.h"

namespace Astra::Graphics
{
	enum EffectType
	{
		DefaultEffect,
		Bloom,
		Hdr,
		Blur
	};

	class ImageEffect
	{
	private:
		size_t m_totalSteps;
		bool m_floating;
	protected:
		EffectType m_type;
		size_t m_step;
		int m_width, m_height;
		FrameBuffer* m_buffer;
		Shader* m_shader;
	public:
		ImageEffect(Shader* shader, size_t totalSteps = 1);
		ImageEffect(Shader* shader, int width, int height, size_t totalSteps = 1, bool floating = false, unsigned int clamping = GL_LINEAR);
		virtual ~ImageEffect();

		inline EffectType GetType() const { return m_type; }

		virtual void Start(unsigned int* attachment);
		virtual void Stop();
		virtual void UpdateAspectRatio(unsigned int width, unsigned int height);
		
		inline bool Finished() { return m_totalSteps == m_step; }
		inline void Reset() { m_step = 0; }
	protected:
		void BindBuffer(unsigned int id) const;
		void UnbindBuffer() const;
	};
}
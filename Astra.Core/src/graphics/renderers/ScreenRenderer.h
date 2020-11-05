#pragma once

#include "Renderer.h"
#include "../buffers/FrameBuffer.h"

namespace Astra::Graphics
{
	class ScreenRenderer : public Renderer
	{
	private:
		const VertexArray* m_defaultQuad;
		const FrameBuffer* m_screenBuffer;
	public:
		ScreenRenderer(Shader* shader);
		~ScreenRenderer();
		
		void Attach();
		void Unattach();

		void Draw(const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector = NULL, const Math::Vec4& clipPlane = DefaultClipPlane) override;
	};
}
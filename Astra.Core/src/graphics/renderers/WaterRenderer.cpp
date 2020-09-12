#include "WaterRenderer.h"
#include "../../math/Mat4Utils.h"
#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	WaterRenderer::WaterRenderer(Shader* shader)
		: Renderer(shader)
	{
		m_defaultQuad = Loader::Load(GL_TRIANGLES, { -1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1 }, 2);
	}

	void WaterRenderer::Draw(const Math::Mat4& viewMatrix)
	{
		m_shader->Start();
		m_shader->SetUniformMat4(Shader::ViewMatrixTag, viewMatrix);

		glBindVertexArray(m_defaultQuad->vaoId);
		glEnableVertexAttribArray(0);

		for (const WaterTile& waterTile : m_waterTiles)
		{
			m_shader->SetUniformMat4(Shader::TransformMatrixTag, Math::Mat4Utils::Transformation(waterTile));
			glDrawArrays(m_defaultQuad->drawType, 0, m_defaultQuad->vertexCount);
		}

		UnbindVertexArray();
		m_shader->Stop();
	}
}

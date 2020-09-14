#include "WaterRenderer.h"
#include "../../math/Mat4Utils.h"
#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	WaterRenderer::WaterRenderer(Shader* shader, Camera* camera)
		: Renderer(shader), m_camera(camera), m_buffer(NULL)
	{
		m_defaultQuad = Loader::Load(GL_TRIANGLES, { -1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1 }, 2);

		m_shader->Start();
		m_shader->SetUniform1i(WaterShader::ReflectionTextureTag, 0);
		m_shader->SetUniform1i(WaterShader::RefractionTextureTag, 1);
		m_shader->Stop();
	}

	void WaterRenderer::Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane)
	{
		m_shader->Start();
		m_shader->SetUniformMat4(Shader::ViewMatrixTag, viewMatrix);

		glBindVertexArray(m_defaultQuad->vaoId);
		glEnableVertexAttribArray(0);
		if (m_buffer)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_buffer->GetReflectionBuffer().GetColorAttachment());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_buffer->GetRefractionBuffer().GetColorAttachment());
		}

		for (const WaterTile& tile: m_waterTiles)
		{
			m_shader->SetUniformMat4(Shader::TransformMatrixTag, Math::Mat4Utils::Transformation(tile));
			glDrawArrays(m_defaultQuad->drawType, 0, m_defaultQuad->vertexCount);
		}

		UnbindVertexArray();
		m_shader->Stop();
	}
}

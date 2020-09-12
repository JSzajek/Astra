#include "SkyboxRenderer.h"

namespace Astra::Graphics
{
	SkyboxRenderer::SkyboxRenderer(Shader* shader)
		: Renderer(shader), m_texture(NULL)
	{
		m_cube = Loader::Load(GL_TRIANGLES, Vertices, 3);
		m_texture = Loader::LoadCubeMap(m_textureFiles);
	}

	void SkyboxRenderer::Draw(const Math::Mat4& viewMatrix)
	{
		m_shader->Start();
		Math::Mat4 copy = Math::Mat4(viewMatrix);
		copy.columns[3].x = 0;
		copy.columns[3].y = 0;
		copy.columns[3].z = 0;
		m_shader->SetUniformMat4(Shader::ViewMatrixTag, copy);
	
		glBindVertexArray(m_cube->vaoId);
		glEnableVertexAttribArray(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture->id);
		glDrawArrays(m_cube->drawType, 0, m_cube->vertexCount);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		m_shader->Stop();
	}
}

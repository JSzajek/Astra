#include "SkyboxRenderer.h"

namespace Astra::Graphics
{
	SkyboxRenderer::SkyboxRenderer(Shader* shader, const Math::Vec3* fogColor)
		: Renderer(shader), m_skyColor(fogColor), m_blendFactor(0), m_material(NULL)
	{
		m_cube = Loader::Load(GL_TRIANGLES, Vertices, 3);

		m_shader->Start();
		m_shader->SetUniform1i(SkyboxShader::FirstCubeMapTag, 0);
		m_shader->SetUniform1i(SkyboxShader::SecondCubeMapTag, 1);
		m_shader->Stop();
	}

	void SkyboxRenderer::Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane)
	{
		m_shader->Start();

		if (m_shader->GetType() == ShaderType::Skybox)
		{
			m_shader->SetUniform3f(SkyboxShader::FogColorTag, *m_skyColor);
		}
		m_shader->SetUniformMat4(Shader::ViewMatrixTag, viewMatrix);
	
		glBindVertexArray(m_cube->vaoId);
		glEnableVertexAttribArray(0);
		BindTextures();
		glDrawArrays(m_cube->drawType, 0, m_cube->vertexCount);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		m_shader->Stop();
	}

	void SkyboxRenderer::BindTextures()
	{
		if (m_material == NULL) { return; }

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_material->GetFirstTextureId());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_material->GetSecondTextureId());
		m_shader->SetUniform1f(SkyboxShader::BlendFactorTag, m_material->GetBlendFactor());
	}
}

#include "TerrainRenderer.h"
#include "../../math/Mat4Utils.h"

namespace Astra::Graphics
{
	TerrainRenderer::TerrainRenderer(Shader* shader, const Math::Vec3* skyColor)
		: Renderer(shader), m_light(Math::Vec3(0), Math::Vec3(0)), m_skyColor(skyColor)
	{
	}

	void TerrainRenderer::Draw(const Math::Mat4& viewMatrix)
	{
		m_shader->Start();
		if (m_shader->GetType() == ShaderType::Terrains)
		{
			m_shader->SetUniform3f(TerrainShader::SkyColorTag, *m_skyColor);
			m_shader->SetUniform3f(TerrainShader::LightPositionTag, m_light.GetTranslation());
			m_shader->SetUniform3f(TerrainShader::LightColorTag, m_light.GetColor());
		}
		m_shader->SetUniformMat4(Shader::ViewMatrixTag, viewMatrix);
		for (const auto& directory : m_terrains)
		{
			std::vector<const Terrain*> terrains = directory.second;
			PrepareTerrain(*terrains.front());
			for (const Terrain* terrain : terrains)
			{
				m_shader->SetUniformMat4(Shader::TransformMatrixTag, Math::Mat4Utils::Transformation(*terrain));
				glDrawElements(terrain->vertexArray->drawType, terrain->vertexArray->vertexCount, GL_UNSIGNED_INT, NULL);
			}
		}
		UnbindVertexArray();
		m_shader->Stop();
	}

	void TerrainRenderer::PrepareTerrain(const Terrain& terrain)
	{
		glBindVertexArray(terrain.vertexArray->vaoId);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		if (terrain.texture->transparent)
		{
			glDisable(GL_CULL_FACE);
		}

		if (m_shader->GetType() == ShaderType::Terrains)
		{
			m_shader->SetUniform1f(TerrainShader::ShineDampenerTag, terrain.texture->shineDampener);
			m_shader->SetUniform1f(TerrainShader::ReflectivityTag, terrain.texture->reflectivity);
		}

		if (terrain.texture != NULL)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, terrain.texture->id);
		}
	}
}
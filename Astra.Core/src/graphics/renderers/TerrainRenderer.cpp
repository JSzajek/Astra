#include "TerrainRenderer.h"
#include "../../math/Mat4Utils.h"

namespace Astra::Graphics
{
	TerrainRenderer::TerrainRenderer(Shader* shader, const Math::Vec3* skyColor)
		: Renderer(shader), m_light(Math::Vec3(0), Math::Vec3(0)), m_skyColor(skyColor)
	{
		m_shader->Start();
		m_shader->SetUniform1i(TerrainShader::BackgroundTextureTag, 0);
		m_shader->SetUniform1i(TerrainShader::RTextureTag, 1);
		m_shader->SetUniform1i(TerrainShader::GTextureTag, 2);
		m_shader->SetUniform1i(TerrainShader::BTextureTag, 3);
		m_shader->SetUniform1i(TerrainShader::BlendMapTag, 4);
		m_shader->Stop();
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

	void TerrainRenderer::BindTerrainTextures(const Terrain& terrain)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, terrain.texturePack->backgroundTexture->id);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, terrain.texturePack->rTexture->id);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, terrain.texturePack->gTexture->id);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, terrain.texturePack->bTexture->id);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, terrain.blendMap->id);
	}

	void TerrainRenderer::PrepareTerrain(const Terrain& terrain)
	{
		glBindVertexArray(terrain.vertexArray->vaoId);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		BindTerrainTextures(terrain);

		// TODO: Add shine and reflectivity back to terrains
		if (m_shader->GetType() == ShaderType::Terrains)
		{
			m_shader->SetUniform1f(TerrainShader::ShineDampenerTag, 1);
			m_shader->SetUniform1f(TerrainShader::ReflectivityTag, 0);
		}
	}
}
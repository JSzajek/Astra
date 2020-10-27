#include "TerrainRenderer.h"
#include "../../math/Mat4Utils.h"

namespace Astra::Graphics
{
	TerrainRenderer::TerrainRenderer(Shader* shader, const Math::Vec3* fogColor)
		: Renderer(shader), m_skyColor(fogColor)
	{
		m_shader->Start();
		m_shader->SetUniform1i(TerrainShader::BackgroundTextureTag, 0);
		m_shader->SetUniform1i(TerrainShader::RTextureTag, 1);
		m_shader->SetUniform1i(TerrainShader::GTextureTag, 2);
		m_shader->SetUniform1i(TerrainShader::BTextureTag, 3);
		m_shader->SetUniform1i(TerrainShader::BlendMapTag, 4);
		m_shader->SetUniform1i(TerrainShader::ShadowMapTag, 5);
		m_shader->Stop();
	}

	void TerrainRenderer::Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane)
	{
		m_shader->Start();
		if (m_shader->GetType() == ShaderType::Terrains)
		{
			m_shader->SetUniform4f(TerrainShader::ClipPaneTag, clipPlane);
			m_shader->SetUniform3f(TerrainShader::SkyColorTag, *m_skyColor);
		}
		m_shader->SetUniformMat4(Shader::ViewMatrixTag, viewMatrix);
		m_shader->SetUniformMat4(TerrainShader::ToShadowSpaceMatrixTag, m_toShadowSpaceMatrix);
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

	void TerrainRenderer::AddTerrain(const Terrain* terrain)
	{
		auto temp = m_terrains.find(terrain->vertexArray->vaoId);
		if (temp != m_terrains.end())
		{
			temp->second.emplace_back(terrain);
		}
		else
		{
			m_terrains[terrain->vertexArray->vaoId] = std::vector<const Terrain*>();
			m_terrains[terrain->vertexArray->vaoId].emplace_back(terrain);
		}
	}

	void TerrainRenderer::AddLight(Light* light)
	{
		if (m_lights.size() + 1 > MAX_LIGHTS)
		{
			Logger::Log("Too Many Lights");
			m_lights.pop_back();
		}
		m_lights.emplace_back(light);
		light->SetCallback(std::bind(&TerrainRenderer::UpdateLights, this));
		UpdateLights();
	}

	void TerrainRenderer::UpdateLights()
	{
		m_shader->Start();
		for (int i = 0; i < MAX_LIGHTS; i++)
		{
			if (i < m_lights.size())
			{
				m_shader->SetUniform3f(TerrainShader::GetLightPositionTag(i), m_lights[i]->GetTranslation());
				m_shader->SetUniform3f(TerrainShader::GetLightColorTag(i), m_lights[i]->GetColor());
				m_shader->SetUniform3f(TerrainShader::GetAttenuationTag(i), m_lights[i]->GetAttenuation());
			}
			else
			{
				m_shader->SetUniform3f(TerrainShader::GetLightPositionTag(i), Math::Vec3(0));
				m_shader->SetUniform3f(TerrainShader::GetLightColorTag(i), Math::Vec3(0));
				m_shader->SetUniform3f(TerrainShader::GetAttenuationTag(i), Math::Vec3(1, 0, 0));
			}
		}
		m_shader->Stop();
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
}
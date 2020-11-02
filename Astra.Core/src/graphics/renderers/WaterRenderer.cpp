#include "WaterRenderer.h"
#include "../../math/Mat4Utils.h"
#include "../loaders/Loader.h"
#include "../Window.h"

#include "../entities/PointLight.h"
#include "../entities/SpotLight.h"

namespace Astra::Graphics
{
	WaterRenderer::WaterRenderer(Camera* camera, const Math::Vec3* fogColor, float near, float far)
		: Renderer(), m_camera(camera), m_fogColor(fogColor), m_buffer(NULL), m_near(near), m_far(far)
	{
		m_defaultQuad = Loader::Load(GL_TRIANGLES, { -1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1 }, 2);
	}

	void WaterRenderer::SetShader(Shader* shader)
	{
		Renderer::SetShader(shader);

		m_shader->Start();
		m_shader->SetUniform1f(NEAR_PLANE, m_near);
		m_shader->SetUniform1f(FAR_PLANE, m_far);

		m_shader->SetUniform1i(REFLECTION_TEXTURE, 0);
		m_shader->SetUniform1i(REFRACTION_TEXTURE, 1);
		m_shader->SetUniform1i(DUDVMAP_TEXTURE, 2);
		m_shader->SetUniform1i(NORMALMAP_TEXTURE, 3);
		m_shader->SetUniform1i(DEPTHMAP_TEXTURE, 4);
		m_shader->SetUniform1i(SPECULAR_MAP, 5);
		m_shader->Stop();
	}

	void WaterRenderer::Clear()
	{
		m_lights.clear();
		m_waterTiles.clear();
	}

	void WaterRenderer::Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane)
	{
		m_shader->Start();
		m_shader->SetUniform3f(FOG_COLOR, *m_fogColor);
		//m_shader->SetUniform3f(CAMERA_POSITION, m_camera->GetTranslation());

		for (int i = 0; i < m_lights.size(); i++)
		{
			m_shader->SetUniform3f(Shader::GetPointLightPositionTag(i), m_lights[i]->GetTranslation());
			m_shader->SetUniform3f(Shader::GetPointLightAmbientTag(i), m_lights[i]->GetAmbient());
			m_shader->SetUniform3f(Shader::GetPointLightDiffuseTag(i), m_lights[i]->GetDiffuse());
			m_shader->SetUniform3f(Shader::GetPointLightSpecularTag(i), m_lights[i]->GetSpecular());
			m_shader->SetUniform3f(Shader::GetPointLightAttenuationTag(i), (static_cast<const PointLight*>(m_lights[i]))->GetAttenuation());
		}

		m_shader->SetUniform3f(DIR_LIGHT_DIRECTION, m_directionalLight->GetRotation());
		m_shader->SetUniform3f(DIR_LIGHT_AMBIENT, m_directionalLight->GetAmbient());
		m_shader->SetUniform3f(DIR_LIGHT_DIFFUSE, m_directionalLight->GetDiffuse());
		m_shader->SetUniform3f(DIR_LIGHT_SPECULAR, m_directionalLight->GetSpecular());

		m_shader->SetUniformMat4(Shader::ViewMatrixTag, viewMatrix);
		m_shader->SetUniform4f(Shader::InverseViewVectorTag, viewMatrix.Inverse() * Math::Back4D);
		
		PrepareRender();
		for (const WaterTile* tile: m_waterTiles)
		{
			PrepareTile(tile);
			m_shader->SetUniform1f(MOVE_FACTOR, tile->material->Increase());
			m_shader->SetUniformMat4(Shader::TransformMatrixTag, Math::Mat4Utils::Transformation(*tile));
			glDrawArrays(m_defaultQuad->drawType, 0, m_defaultQuad->vertexCount);
		}
		UnbindVertexArray();
		m_shader->Stop();
	}

	void WaterRenderer::AddLight(const Light* light)
	{
		m_lights.emplace_back(light);
	}

	void WaterRenderer::PrepareRender()
	{
		glBindVertexArray(m_defaultQuad->vaoId);
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		/*if (m_light)
		{
			m_shader->SetUniform3f(WaterShader::LightPositionTag, m_light->GetTranslation());
			m_shader->SetUniform3f(WaterShader::LightColorTag, m_light->GetColor());
		}*/
		if (m_buffer)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_buffer->GetReflectionBuffer().GetColorAttachment());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_buffer->GetRefractionBuffer().GetColorAttachment());
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, m_buffer->GetRefractionBuffer().GetDepthAttachment());
		}
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void WaterRenderer::PrepareTile(const WaterTile* tile)
	{
		m_shader->SetUniform1f(WAVE_STRENGTH, tile->material->waveStrength);
		m_shader->SetUniform4f(BASE_WATER_COLOR, tile->material->baseColor);
		m_shader->SetUniform1f(MATERIAL_REFLECTIVITY, tile->material->reflectivity);
		//m_shader->SetUniform1f(WaterShader::ShineDampenerTag, tile->material->shineDampener);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, tile->material->dudvTexture.id);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, tile->material->normalTexture.id);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, tile->material->GetSpecularId());
	}

	void WaterRenderer::UnbindVertexArray()
	{
		glDisable(GL_BLEND);
		Renderer::UnbindVertexArray();
	}
}

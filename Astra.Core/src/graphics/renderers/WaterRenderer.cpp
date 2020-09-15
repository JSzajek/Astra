#include "WaterRenderer.h"
#include "../../math/Mat4Utils.h"
#include "../loaders/Loader.h"
#include "../Window.h"

namespace Astra::Graphics
{
	WaterRenderer::WaterRenderer(Shader* shader, Camera* camera)
		: Renderer(shader), m_camera(camera), m_buffer(NULL), m_light(NULL),
			dudvMap(Loader::LoadTexture("res/textures/waterDUDV.png")),
			normalMap(Loader::LoadTexture("res/textures/NormalMap.png")),
			m_currentMoveFactor(0), m_waveSpeed(DefaultWaveSpeed)
	{
		m_defaultQuad = Loader::Load(GL_TRIANGLES, { -1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1 }, 2);

		m_shader->Start();
		m_shader->SetUniform1i(WaterShader::ReflectionTextureTag, 0);
		m_shader->SetUniform1i(WaterShader::RefractionTextureTag, 1);
		m_shader->SetUniform1i(WaterShader::DuDvMapTextureTag, 2);
		m_shader->SetUniform1i(WaterShader::NormalMapTextureTag, 3);
		m_shader->SetUniform1i(WaterShader::DepthMapTextureTag, 4);
		m_shader->Stop();
	}

	void WaterRenderer::Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane)
	{
		m_shader->Start();
		m_shader->SetUniformMat4(Shader::ViewMatrixTag, viewMatrix);
		
		m_currentMoveFactor += m_waveSpeed * Window::delta;
		m_currentMoveFactor = fmod(m_currentMoveFactor, 1);
		m_shader->SetUniform1f(WaterShader::MoveFactorTag, m_currentMoveFactor);
		m_shader->SetUniform3f(WaterShader::CameraPositionTag, m_camera->GetTranslation());

		if (m_light)
		{
			m_shader->SetUniform3f(WaterShader::LightPositionTag, m_light->GetTranslation());
			m_shader->SetUniform3f(WaterShader::LightColorTag, m_light->GetColor());
		}
		PrepareRender();
		for (const WaterTile& tile: m_waterTiles)
		{
			m_shader->SetUniformMat4(Shader::TransformMatrixTag, Math::Mat4Utils::Transformation(tile));
			glDrawArrays(m_defaultQuad->drawType, 0, m_defaultQuad->vertexCount);
		}
		UnbindVertexArray();
		m_shader->Stop();
	}

	void WaterRenderer::PrepareRender()
	{
		glBindVertexArray(m_defaultQuad->vaoId);
		glEnableVertexAttribArray(0);
		if (m_buffer)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_buffer->GetReflectionBuffer().GetColorAttachment());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_buffer->GetRefractionBuffer().GetColorAttachment());
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, dudvMap.id);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, normalMap.id);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, m_buffer->GetRefractionBuffer().GetDepthAttachment());
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void WaterRenderer::UnbindVertexArray()
	{
		glDisable(GL_BLEND);
		Renderer::UnbindVertexArray();
	}
}

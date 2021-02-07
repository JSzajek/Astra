#include "SkyboxRenderer.h"

#include "../ResourceManager.h"

namespace Astra::Graphics
{
	SkyboxRenderer::SkyboxRenderer(Shader* shader, const Math::Vec3* fogColor)
		: Renderer(), m_fogColor(fogColor), m_blendFactor(0), m_material(NULL), m_rotation(0), m_fixedViewMatrix(new Math::Mat4())
	{
		Renderer::SetShader(shader);

		m_shader->Start();
		m_shader->SetUniform1i(FIRST_CUBE_MAP_TAG,		0);
		m_shader->SetUniform1i(SECOND_CUBE_MAP_TAG,		1);
		m_shader->Stop();

		m_cube = Loader::Load(GL_TRIANGLES, Vertices, 3);
	}

	SkyboxRenderer::~SkyboxRenderer()
	{
		ResourceManager::Unload(m_cube);
		delete m_material;
	}

	void SkyboxRenderer::Draw(float delta, const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane)
	{
		m_shader->Start();
		m_shader->SetUniform3f(FOG_COLOR, *m_fogColor);

		// Set Custom View Matrix
		*m_fixedViewMatrix = Math::Mat4(*viewMatrix); // Should look into better way besides copying
		m_fixedViewMatrix->columns[3].x = 0;
		m_fixedViewMatrix->columns[3].y = 0;
		m_fixedViewMatrix->columns[3].z = 0;
		m_rotation += RotationSpeed * delta;
		m_fixedViewMatrix->Rotate(m_rotation, Math::Vec3::Y_Axis);
		m_shader->SetUniformMat4(VIEW_MATRIX_TAG, m_fixedViewMatrix);
	
		glBindVertexArray(m_cube->vaoId);
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		glEnableVertexAttribArray(0);
		BindTextures();
		glDrawArrays(m_cube->drawType, 0, m_cube->vertexCount);
		glDepthFunc(GL_LESS); // set depth function back to default
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		m_shader->Stop();
	#if _DEBUG
		glCheckError();
	#endif
	}

	void SkyboxRenderer::BindTextures()
	{
		if (m_material == NULL) { return; }

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_material->GetFirstTextureId());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_material->GetSecondTextureId());
		m_shader->SetUniform1f(BLEND_FACTOR_TAG, m_material->GetBlendFactor());
	}
}

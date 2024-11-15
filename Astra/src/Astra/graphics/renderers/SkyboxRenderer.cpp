#include "astra_pch.h"

#include "SkyboxRenderer.h"
#include "Astra/graphics/Resource.h"

namespace Astra::Graphics
{
	SkyboxRenderer::SkyboxRenderer(Shader* shader, const Color* fogColor)
		: Renderer(), m_fogColor(fogColor), m_blendFactor(0), m_rotation(0), m_fixedViewMatrix(new Math::Mat4()), m_set(false)
	{
		Renderer::SetShader(shader);

		m_shader->Start();
		m_shader->SetUniform1i(FIRST_CUBE_MAP_TAG,		0);
		m_shader->SetUniform1i(SECOND_CUBE_MAP_TAG,		1);
		m_shader->Stop();

		m_cube = Resource::LoadMesh(MeshCreationSpec("Skybox_Renderer_Default_Cube", GL_TRIANGLES, &Vertices, 3));
	}

	void SkyboxRenderer::SetSkyBox(const SkyboxMaterial& material)
	{
		m_set = true;
		m_material = material;
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
	
		glBindVertexArray(m_cube->GetVAO());
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		BindTextures();
		glDrawArrays(m_cube->GetDrawType(), 0, m_cube->GetVertexCount());
		glDepthFunc(GL_LESS); // set depth function back to default
		glBindVertexArray(0);
		m_shader->Stop();
	#if ASTRA_DEBUG
		glCheckError();
	#endif
	}

	void SkyboxRenderer::BindTextures()
	{
		if (m_set)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_material.GetFirstTextureId());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_material.GetSecondTextureId());
			m_shader->SetUniform1f(BLEND_FACTOR_TAG, m_material.GetBlendFactor());
		}
	}
}

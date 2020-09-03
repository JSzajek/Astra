#include "Renderer.h"

namespace Astra::Graphics
{
	Renderer::Renderer(Shader* shader)
		: m_shader(shader)
	{
	}

	Renderer::~Renderer()
	{
		delete m_shader;
	}

	void Renderer::UpdateProjectionMatrix(const Math::Mat4& projectionMatrix)
	{
		m_shader->Start();
		m_shader->SetUniformMat4(Shader::ProjectionMatrixTag, projectionMatrix);
		m_shader->Stop();
	}

	void Renderer::UnbindTextureModel()
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glDisableVertexAttribArray(BufferType::Vertices);
		glDisableVertexAttribArray(BufferType::TextureCoords);
		glDisableVertexAttribArray(BufferType::Normals);
		glBindVertexArray(0);
	}

	void Renderer::UpdateDynamicVbo()
	{

	}

	void Renderer::UpdateTexture()
	{

	}
}
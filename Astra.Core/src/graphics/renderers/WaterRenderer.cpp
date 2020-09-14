#include "WaterRenderer.h"
#include "../../math/Mat4Utils.h"
#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	WaterRenderer::WaterRenderer(Shader* shader, Camera* camera, std::function<void(const Math::Vec4&)> renderCallback)
		: Renderer(shader), m_renderCallback(renderCallback), m_camera(camera),
			m_reflectionClipPlane(Math::Vec4(0, 1, 0, 0)),
			m_refractionClipPlane(Math::Vec4(0, -1, 0, 0))
	{
		m_defaultQuad = Loader::Load(GL_TRIANGLES, { -1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1 }, 2);
	}

	void WaterRenderer::Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane)
	{
		m_shader->Start();
		m_shader->SetUniformMat4(Shader::ViewMatrixTag, viewMatrix);

		glBindVertexArray(m_defaultQuad->vaoId);
		glEnableVertexAttribArray(0);

		for (const auto& directory : m_waterTiles)
		{
			const WaterFrameBuffer& buffer = directory.first;
			std::vector<WaterTile> tiles = directory.second;

			if (tiles.size() == 0) { continue; }

			m_shader->Stop();

			float distance = 2 * (m_camera->GetTranslation().y - tiles[0].GetTranslation().y);
			m_reflectionClipPlane.w = -tiles[0].GetTranslation().y;
			m_refractionClipPlane.w = tiles[0].GetTranslation().y;
			
			BindFrameBuffer(buffer.GetReflectionBuffer().GetId(), 320, 180);
			m_camera->Translation().y -= distance;
			m_camera->InvertPitch();
			m_renderCallback(m_reflectionClipPlane);
			UnbindFrameBuffer();


			BindFrameBuffer(buffer.GetRefractionBuffer().GetId(), 1280, 720);
			m_camera->Translation().y += distance;
			m_camera->InvertPitch();
			m_renderCallback(m_refractionClipPlane);
			UnbindFrameBuffer();

			m_shader->Start();

			for (const WaterTile& tile: tiles)
			{
				m_shader->SetUniformMat4(Shader::TransformMatrixTag, Math::Mat4Utils::Transformation(tile));
				glDrawArrays(m_defaultQuad->drawType, 0, m_defaultQuad->vertexCount);
			}
		}

		UnbindVertexArray();
		m_shader->Stop();
	}

	void WaterRenderer::AddTile(const WaterFrameBuffer& buffer, const WaterTile& tile)
	{
		auto temp = m_waterTiles.find(buffer);
		if (temp != m_waterTiles.end())
		{
			temp->second.emplace_back(tile);
		}
		else
		{
			m_waterTiles[buffer] = std::vector<WaterTile>();
			m_waterTiles[buffer].emplace_back(tile);
		}
	}
}

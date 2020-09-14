#include "RendererController.h"

#include <functional>

namespace Astra::Graphics
{
	RendererController::RendererController(const Math::Vec3& fogColor)
		: fogColor(fogColor)
	{
		Init();
		m_guiShader = new GuiShader();
		m_guiRenderer = new GuiRenderer(m_guiShader);

		m_basicShader = new BasicShader();
		m_lightingShader = new LightingShader();
		m_entityRenderer = new Entity3dRenderer(m_lightingShader, &fogColor);

		m_terrainShader = new TerrainShader();
		m_terrainRenderer = new TerrainRenderer(m_terrainShader, &fogColor);

		m_skyboxShader = new SkyboxShader();
		m_skyboxRenderer = new SkyboxRenderer(m_skyboxShader, &fogColor);

		m_waterShader = new WaterShader();
		m_waterRenderer = new WaterRenderer(m_waterShader, m_mainCamera, [&](const Math::Vec4& clipPlane)
		{
			UpdateCameraView();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_terrainRenderer->Draw(viewMatrix, clipPlane);
			m_entityRenderer->Draw(viewMatrix, clipPlane);
			m_skyboxRenderer->Draw(viewMatrix);
		});

		modelViewMatrix = Math::Mat4::Identity();
	}

	void RendererController::Init() const
	{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glClearColor(fogColor.x, fogColor.y, fogColor.z, 1);

		glEnable(GL_CLIP_DISTANCE0);
	}

	RendererController::~RendererController()
	{
		delete m_guiRenderer;
		delete m_entityRenderer;
	}

	void RendererController::UpdateScreen(float width, float height)
	{
		projectionMatrix = Math::Mat4::Perspective(width, height, FieldOfView, NearPlane, FarPlane);
		m_entityRenderer->UpdateProjectionMatrix(projectionMatrix);
		m_terrainRenderer->UpdateProjectionMatrix(projectionMatrix);
		m_skyboxRenderer->UpdateProjectionMatrix(projectionMatrix);
		m_waterRenderer->UpdateProjectionMatrix(projectionMatrix);
	}

	void RendererController::Render()
	{
		UpdateCameraView();
		
		PreRender();

		PostRender();
		GuiRender();
	}

	void RendererController::PostRender(const Math::Vec4& clipPlane)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_terrainRenderer->Draw(viewMatrix);
		m_entityRenderer->Draw(viewMatrix);
		m_skyboxRenderer->Draw(viewMatrix);
	}

	void RendererController::PreRender()
	{
		m_waterRenderer->Draw(viewMatrix);
	}

	void RendererController::GuiRender()
	{
		m_guiRenderer->Draw(NULL);
	}

	void RendererController::UpdateCameraView()
	{
		if (m_mainCamera != nullptr)
		{
			m_mainCamera->UpdatePosition();
			viewMatrix = Math::Mat4Utils::ViewMatrix(*m_mainCamera);
		}
		else
		{
			Logger::LogWarning("No Main Camera detected.");
		}
	}
}
#include "RendererController.h"

namespace Astra::Graphics
{
	RendererController::RendererController()
		: m_light(Light(Math::Vec3(20000, 20000, 2000), Math::Vec3(1, 1, 1))),
			skyColor(Math::Vec3(0.1f, 0.7f, 0.7f))
	{
		Init();

		m_guiShader = new GuiShader();
		m_guiRenderer = new GuiRenderer(m_guiShader);

		m_basicShader = new BasicShader();
		m_lightingShader = new LightingShader();
		m_entityRenderer = new Entity3dRenderer(m_lightingShader, &skyColor);

		m_terrainShader = new TerrainShader();
		m_terrainRenderer = new TerrainRenderer(m_terrainShader, &skyColor);

		modelViewMatrix = Math::Mat4::Identity();

		m_entityRenderer->AddLight(m_light);
		m_terrainRenderer->AddLight(m_light);
	}

	void RendererController::Init() const
	{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glClearColor(skyColor.x, skyColor.y, skyColor.z, 1);
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
	}

	void RendererController::Render()
	{
		UpdateCameraView();

		m_terrainRenderer->Draw(viewMatrix);
		m_entityRenderer->Draw(viewMatrix);
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
			Log::Logger::LogWarning("No Main Camera detected.");
		}
	}
}
#include "RendererController.h"

#include <functional>

namespace Astra::Graphics
{
	RendererController::RendererController(const Math::Vec3& fogColor)
		: fogColor(fogColor), m_reflectionClipPlane(Math::Vec4(0, 1, 0, 0)),
			m_refractionClipPlane(Math::Vec4(0, -1, 0, 0))
	{
		Init();
		m_guiShader = new GuiShader();
		m_guiRenderer = new GuiRenderer(m_guiShader);

		m_lightingShader = new LightingShader();
		m_entityRenderer = new Entity3dRenderer(m_lightingShader, &fogColor);

		m_terrainShader = new TerrainShader();
		m_terrainRenderer = new TerrainRenderer(m_terrainShader, &fogColor);

		m_skyboxShader = new SkyboxShader();
		m_skyboxRenderer = new SkyboxRenderer(m_skyboxShader, &fogColor);

		m_waterShader = new WaterShader();
		m_waterRenderer = new WaterRenderer(m_waterShader, m_mainCamera, NearPlane, FarPlane);

		m_normalEntityShader = new NormalEntityShader();
		m_normalEntityRenderer = new NormalEntity3dRenderer(m_normalEntityShader, &fogColor);

		m_waterBuffer = Loader::LoadWaterFrameBuffer(DefaultReflectionWidth, DefaultReflectionHeight,
													 DefaultRefractionWidth, DefaultRefractionHeight);
		m_waterRenderer->SetFrameBuffer(m_waterBuffer);

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
		delete m_terrainRenderer;
		delete m_skyboxRenderer;
		delete m_waterRenderer;
		delete m_normalEntityRenderer;
	}

	void RendererController::UpdateScreen(float width, float height)
	{
		projectionMatrix = Math::Mat4::Perspective(width, height, FieldOfView, NearPlane, FarPlane);
		m_terrainRenderer->UpdateProjectionMatrix(projectionMatrix);
		m_entityRenderer->UpdateProjectionMatrix(projectionMatrix);
		m_normalEntityRenderer->UpdateProjectionMatrix(projectionMatrix);
		m_skyboxRenderer->UpdateProjectionMatrix(projectionMatrix);
		m_waterRenderer->UpdateProjectionMatrix(projectionMatrix);
		ParticleController::UpdateProjectionMatrix(projectionMatrix);
	}

	void RendererController::Render()
	{
		if (m_waterBuffer && m_mainCamera)
		{
			float distance = 2 * (m_mainCamera->GetTranslation().y - m_refractionClipPlane.w);

			m_waterRenderer->BindFrameBuffer(m_waterBuffer->GetReflectionBuffer().GetId(), 320, 180);
			m_mainCamera->Translation().y -= distance;
			m_mainCamera->InvertPitch();
			viewMatrix = Math::Mat4Utils::ViewMatrix(*m_mainCamera);
			PreRender(m_reflectionClipPlane);
			m_waterRenderer->UnbindFrameBuffer();

			m_mainCamera->Translation().y += distance;
			m_mainCamera->InvertPitch();
			viewMatrix = Math::Mat4Utils::ViewMatrix(*m_mainCamera);
			m_waterRenderer->BindFrameBuffer(m_waterBuffer->GetRefractionBuffer().GetId(), 1280, 720);
			PreRender(m_refractionClipPlane);
			m_waterRenderer->UnbindFrameBuffer();
		}

		UpdateCameraView();
		ParticleController::Update(m_mainCamera->GetTranslation());
		PreRender();
		PostRender();
		GuiRender();
	}

	void RendererController::PreRender(const Math::Vec4& clipPlane)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_terrainRenderer->Draw(viewMatrix, clipPlane);
		m_entityRenderer->Draw(viewMatrix, clipPlane);
		m_normalEntityRenderer->Draw(viewMatrix, clipPlane);
		m_skyboxRenderer->Draw(viewMatrix);
	}

	void RendererController::PostRender()
	{
		m_waterRenderer->Draw(viewMatrix);
	}

	void RendererController::GuiRender()
	{
		ParticleController::Render(viewMatrix);
		m_guiRenderer->Draw(NULL);
		FontController::Render();
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
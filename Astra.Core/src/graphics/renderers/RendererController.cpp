#include "RendererController.h"

#include "../Window.h"

#include <functional>

namespace Astra::Graphics
{
	RendererController::RendererController()
		: m_reflectionClipPlane(Math::Vec4(0, 1, 0, 0)),
			m_refractionClipPlane(Math::Vec4(0, -1, 0, 0)),
			modelViewMatrix(1), m_block(false)
	{
		m_fogColor = new Math::Vec3(0);

		Init();
		m_shadowMapController = new ShadowMapController(FieldOfView, NearPlane, FarPlane);
		
		m_guiRenderer = new GuiRenderer(new GuiShader());
		m_skyboxRenderer = new SkyboxRenderer(new SkyboxShader(), m_fogColor);
		m_entityRenderer = new Entity3dRenderer(m_fogColor);
		m_terrainRenderer = new TerrainRenderer(m_fogColor);
		m_waterRenderer = new WaterRenderer(m_mainCamera, NearPlane, FarPlane);
		m_normalEntityRenderer = new NormalEntity3dRenderer(m_fogColor);
		
		m_waterBuffer = Loader::LoadWaterFrameBuffer(DefaultReflectionWidth, DefaultReflectionHeight,
													 DefaultRefractionWidth, DefaultRefractionHeight);
		m_waterRenderer->SetFrameBuffer(m_waterBuffer);
	}

	void RendererController::Init() const
	{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glClearColor(m_fogColor->x, m_fogColor->y, m_fogColor->z, 1);

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
		delete m_shadowMapController;
	}

	bool RendererController::SetCurrentSceneImpl(const Scene* scene)
	{
		m_block = true;
		m_currentScene = scene;

		// Create Shaders
		auto& pointLights = scene->GetPointLights();

		m_entityRenderer->SetShader(new EntityShader(pointLights.size()));
		m_terrainRenderer->SetShader(new TerrainShader());
		m_normalEntityRenderer->SetShader(new NormalEntityShader());
		m_waterRenderer->SetShader(new WaterShader());

		// Clear Renderers
		Clear();

		// Pass new Scene information
		m_mainCamera = scene->GetCamera();
		m_shadowMapController->SetCamera(m_mainCamera);
		m_waterRenderer->SetCamera(m_mainCamera);

		m_entityRenderer->AddDirectionalLight(scene->GetDirectionalLight());

		Math::Vec3 fogColor = scene->GetFogColor();
		m_fogColor->x = fogColor.x;
		m_fogColor->y = fogColor.y;
		m_fogColor->z = fogColor.z;

		for (auto* entity : scene->GetEntities())
		{
			m_entityRenderer->AddEntity(entity);
			m_shadowMapController->AddEntity(entity);
		}

		for (auto* light : scene->GetPointLights())
		{
			m_entityRenderer->AddLight(light);
		}

		for (auto* terrain : scene->GetTerrains())
		{
			m_terrainRenderer->AddTerrain(terrain);
		}

		for (auto* gui : scene->GetGuis())
		{
			m_guiRenderer->AddGui(gui);
		}

		m_block = false;

		// Update Projection Matrix
		UpdateScreenImpl(Window::width, Window::height);

		return true;
	}

	void RendererController::Clear()
	{
		m_shadowMapController->Clear();

		m_skyboxRenderer->Clear();
		m_guiRenderer->Clear();
		m_entityRenderer->Clear();
		m_terrainRenderer->Clear();
		m_normalEntityRenderer->Clear();
		m_waterRenderer->Clear();
		FontController::Clear();
		ParticleController::Clear();
	}

	void RendererController::UpdateScreenImpl(float width, float height)
	{
		if (m_currentScene == NULL || m_block) { return; }

		projectionMatrix = Math::Mat4::Perspective(width, height, FieldOfView, NearPlane, FarPlane);
		m_terrainRenderer->UpdateProjectionMatrix(projectionMatrix);
		m_entityRenderer->UpdateProjectionMatrix(projectionMatrix);
		m_normalEntityRenderer->UpdateProjectionMatrix(projectionMatrix);
		m_skyboxRenderer->UpdateProjectionMatrix(projectionMatrix);
		m_waterRenderer->UpdateProjectionMatrix(projectionMatrix);
		ParticleController::UpdateProjectionMatrix(projectionMatrix);
	}

	void RendererController::RenderImpl()
	{
		if (m_currentScene == NULL || m_block) { return; }

		m_shadowMapController->Render();

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
		PrepareRender();
		PreRender();
		PostRender();
		GuiRender();
	}

	void RendererController::PrepareRender()
	{
		if (m_currentScene == NULL || m_block) { return; }

		ParticleController::Update(m_mainCamera->GetTranslation());

		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, m_shadowMapController->GetShadowMap());
		const Math::Mat4& toShadowMap = m_shadowMapController->GetToShadowMapSpaceMatrix();
		m_terrainRenderer->SetShadowMatrix(toShadowMap);
		m_entityRenderer->SetShadowMatrix(toShadowMap);
		m_normalEntityRenderer->SetShadowMatrix(toShadowMap);
	}

	void RendererController::PreRender(const Math::Vec4& clipPlane)
	{
		if (m_currentScene == NULL || m_block) { return; }

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		m_terrainRenderer->Draw(viewMatrix, clipPlane);
		m_entityRenderer->Draw(viewMatrix, clipPlane);
		m_normalEntityRenderer->Draw(viewMatrix, clipPlane);
		m_skyboxRenderer->Draw(viewMatrix, NULL);
	}

	void RendererController::PostRender()
	{
		if (m_currentScene == NULL || m_block) { return; }

		m_waterRenderer->Draw(viewMatrix);
	}

	void RendererController::GuiRender()
	{
		if (m_currentScene == NULL || m_block) { return; }

		ParticleController::Render(viewMatrix);
		m_guiRenderer->Draw(NULL);
		FontController::Render();
	}

	void RendererController::UpdateCameraView()
	{
		if (m_currentScene == NULL || m_block) { return; }

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
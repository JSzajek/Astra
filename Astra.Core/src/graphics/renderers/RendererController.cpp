#include "RendererController.h"

#include "../Window.h"

#include <functional>

namespace Astra::Graphics
{
	RendererController::RendererController()
		: m_reflectionClipPlane(Math::Vec4(0, 1, 0, 0)),
			m_refractionClipPlane(Math::Vec4(0, -1, 0, 0)), 
			projectionMatrix(new Math::Mat4(1)), m_block(false)
	{
		m_fogColor = new Math::Vec3(0);

		Init();
		m_postProcessor = new PostProcessor();
		m_shadowMapController = new ShadowMapController(FieldOfView, NearPlane, FarPlane);
		
		m_guiRenderer = new GuiRenderer(new GuiShader());
		m_skyboxRenderer = new SkyboxRenderer(new SkyboxShader(), m_fogColor);
		m_entityRenderer = new Entity3dRenderer(m_fogColor);
		m_terrainRenderer = new TerrainRenderer(m_fogColor);
		m_normalEntityRenderer = new NormalEntity3dRenderer(m_fogColor);
		m_waterRenderer = new WaterRenderer(NearPlane, FarPlane);

		m_waterBuffer = Loader::LoadWaterFrameBuffer(DefaultReflectionWidth, DefaultReflectionHeight,
													 DefaultRefractionWidth, DefaultRefractionHeight);
		m_waterRenderer->SetFrameBuffer(m_waterBuffer);
	}

	void RendererController::Init() const
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

	#if FULL_SELECTION
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	#else
		glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
	#endif
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
		size_t numOfLights = pointLights.size();
		m_entityRenderer->SetShader(new EntityShader(numOfLights));
		m_terrainRenderer->SetShader(new TerrainShader(numOfLights));
		m_normalEntityRenderer->SetShader(new NormalEntityShader(numOfLights));
		m_waterRenderer->SetShader(new WaterShader(numOfLights));

		// Clear Renderers
		Clear();

		// Pass new Scene information
		m_mainCamera = scene->GetCamera();
		m_shadowMapController->SetCamera(m_mainCamera);
		viewMatrix = m_mainCamera->GetViewMatrix();

		m_skyboxRenderer->SetSkyBox(scene->GetSkyBox());

		auto* dirLight = scene->GetDirectionalLight();
		m_shadowMapController->SetDirectionalLight(dirLight);
		m_entityRenderer->AddLight(dirLight);
		m_terrainRenderer->AddLight(dirLight);
		m_normalEntityRenderer->AddLight(dirLight);
		m_waterRenderer->AddLight(dirLight);
	#if _DEBUG
		GizmoController::AddGizmo(dirLight->GetGizmo());
	#endif

		Math::Vec3 fogColor = scene->GetFogColor();
		m_fogColor->x = fogColor.x;
		m_fogColor->y = fogColor.y;
		m_fogColor->z = fogColor.z;

		for (auto* entity : scene->GetEntities())
		{
			if (entity->IsNormalMapped() || entity->IsParallaxMapped())
			{
				m_normalEntityRenderer->AddEntity(entity);
			}
			else
			{
				m_entityRenderer->AddEntity(entity);
			}
			m_shadowMapController->AddEntity(entity);
		}
		for (auto* light : scene->GetPointLights())
		{
			m_entityRenderer->AddLight(light);
			m_normalEntityRenderer->AddLight(light);
			m_terrainRenderer->AddLight(light);
			m_waterRenderer->AddLight(light);
		#if _DEBUG
			GizmoController::AddGizmo(light->GetGizmo());
		#endif
		}
		for (auto* terrain : scene->GetTerrains())
		{
			m_terrainRenderer->AddTerrain(terrain);
		}
		for (auto* gui : scene->GetGuis())
		{
			m_guiRenderer->AddGui(gui);
		}
		for (auto* text : scene->GetTexts())
		{
			FontController::LoadText(text);
		}
		for (auto* tile : scene->GetWaterTiles())
		{
			// Determine better method of offseting clipping planes.
			m_reflectionClipPlane.w = -tile->GetTranslation().y + 0.5f;
			m_refractionClipPlane.w = tile->GetTranslation().y + 0.5f;

			m_waterRenderer->AddTile(tile);
		}
		for (auto* system : scene->GetParticles())
		{
			m_systems.emplace_back(system);
		#if _DEBUG
			GizmoController::AddGizmo(system->GetGizmo());
		#endif
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
		m_systems.clear();
	#if _DEBUG
		GizmoController::Clear();
	#endif
	}

	void RendererController::UpdateScreenImpl(int width, int height)
	{
		if (m_currentScene == NULL || m_block) { return; }

		*projectionMatrix = Math::Mat4::Perspective(static_cast<float>(width), static_cast<float>(height), FieldOfView, NearPlane, FarPlane);
		m_terrainRenderer->UpdateProjectionMatrix(projectionMatrix);
		m_entityRenderer->UpdateProjectionMatrix(projectionMatrix);
		m_normalEntityRenderer->UpdateProjectionMatrix(projectionMatrix);
		m_skyboxRenderer->UpdateProjectionMatrix(projectionMatrix);
		m_waterRenderer->UpdateProjectionMatrix(projectionMatrix);
		m_postProcessor->UpdateScreenRatio(width, height);
		ParticleController::UpdateProjectionMatrix(projectionMatrix);
	#if _DEBUG
		GizmoController::UpdateProjectionMatrix(projectionMatrix);
	#endif
	}

	void RendererController::RenderImpl()
	{
		if (m_currentScene == NULL || m_block) { return; }
		
		m_shadowMapController->Render();

		for (auto* system : m_systems)
		{
			system->GenerateParticles();
		}

		if (m_waterBuffer && m_mainCamera)
		{
			float distance = 2 * (m_mainCamera->GetTranslation().y - m_refractionClipPlane.w);

			m_waterRenderer->BindFrameBuffer(m_waterBuffer->GetReflectionBuffer()->GetId(), 320, 180);
			m_mainCamera->Translation()->y -= distance;
			m_mainCamera->InvertPitch(); // Updates the view matrix
			PreRender(viewMatrix->Inverse() * Math::Vec4::W_Axis, m_reflectionClipPlane);
			m_waterRenderer->UnbindFrameBuffer();

			m_mainCamera->Translation()->y += distance;
			m_mainCamera->InvertPitch(); // Updates the view matrix
			m_waterRenderer->BindFrameBuffer(m_waterBuffer->GetRefractionBuffer()->GetId(), 1280, 720);
			PreRender(viewMatrix->Inverse() * Math::Vec4::W_Axis, m_refractionClipPlane);
			m_waterRenderer->UnbindFrameBuffer();
		}

		Math::Vec4 inverseView = viewMatrix->Inverse() * Math::Vec4::W_Axis;
		PrepareRender();
		PreRender(inverseView);
		PostRender(inverseView);
		GuiRender();
	}

	void RendererController::PrepareRender()
	{
		if (m_currentScene == NULL || m_block) { return; }

		ParticleController::Update(m_mainCamera->GetTranslation());
		
		m_postProcessor->Attach();

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, m_shadowMapController->GetShadowMap());

		const Math::Mat4& toShadowMap = m_shadowMapController->GetToShadowMapSpaceMatrix();
		m_terrainRenderer->SetShadowMatrix(toShadowMap);
		m_entityRenderer->SetShadowMatrix(toShadowMap);
		m_normalEntityRenderer->SetShadowMatrix(toShadowMap);
		m_waterRenderer->SetShadowMatrix(toShadowMap);
	}

	void RendererController::PreRender(const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane)
	{
		if (m_currentScene == NULL || m_block) { return; }

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		m_terrainRenderer->Draw(viewMatrix, inverseViewVector, clipPlane);
		m_entityRenderer->Draw(viewMatrix, inverseViewVector, clipPlane);
		m_normalEntityRenderer->Draw(viewMatrix, inverseViewVector, clipPlane);
		m_skyboxRenderer->Draw(viewMatrix, NULL);
	}

	void RendererController::PostRender(const Math::Vec4& inverseViewVector)
	{
		if (m_currentScene == NULL || m_block) { return; }

		m_waterRenderer->Draw(viewMatrix, inverseViewVector);
	}

	void RendererController::GuiRender()
	{
		if (m_currentScene == NULL || m_block) { return; }

		ParticleController::Render(viewMatrix);

		// Perform Post Processing Effects
		m_postProcessor->Detach();
		m_postProcessor->Draw();
		
	#if _DEBUG
		GizmoController::Render(viewMatrix);
	#endif
		m_guiRenderer->Draw(NULL);
		FontController::Render();
	}
}
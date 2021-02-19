#include "astra_pch.h"

#include "Layer3D.h"

#include "Astra/Application.h"

#include "Astra/graphics/loaders/Loader.h"

#include "Astra/graphics/shaders/EntityShader.h"
#include "Astra/graphics/shaders/NormalEntityShader.h"
#include "Astra/graphics/shaders/TerrainShader.h"
#include "Astra/graphics/shaders/SkyboxShader.h"
#include "Astra/graphics/shaders/WaterShader.h"
#include "Astra/graphics/shaders/GizmoShader.h"

namespace Astra
{
	Layer3D::Layer3D()
		: m_reflectionClipPlane(Math::Vec4(0, 1, 0, 0)), m_refractionClipPlane(Math::Vec4(0, -1, 0, 0)),
			m_projectionMatrix(new Math::Mat4(1)), m_toShadowMapMatrix(new Math::Mat4(1)),
			m_fogColor(new Graphics::Color(0.5f, 0.6f, 0.6f, 1.0f)), 
			m_viewMatrix(NULL), m_mainCamera(NULL), m_skybox(NULL), m_mainLight(NULL),
			m_postProcessor(NULL)

	{
		Init();

		// TODO: Load from app settings
		SetMultisampling(Application::Get().GetWindow().IsMultisampling());

		m_selectionRenderer = new Graphics::SelectionRenderer();
		m_shadowMapController = new Graphics::ShadowMapController(FieldOfView, NearPlane, FarPlane);

		m_skyboxRenderer = new Graphics::SkyboxRenderer(new Graphics::SkyboxShader(), m_fogColor);
		m_entityRenderer = new Graphics::Entity3dRenderer(m_fogColor);
		m_terrainRenderer = new Graphics::TerrainRenderer(m_fogColor);
		m_normalEntityRenderer = new Graphics::NormalEntity3dRenderer(m_fogColor);
		m_waterRenderer = new Graphics::WaterRenderer(NearPlane, FarPlane);

		m_waterBuffer = Graphics::Loader::LoadWaterFrameBuffer(DefaultReflectionWidth, DefaultReflectionHeight,
																DefaultRefractionWidth, DefaultRefractionHeight);
		m_waterRenderer->SetFrameBuffer(m_waterBuffer);

		m_terrainRenderer->SetShadowMatrix(m_toShadowMapMatrix);
		m_entityRenderer->SetShadowMatrix(m_toShadowMapMatrix);
		m_normalEntityRenderer->SetShadowMatrix(m_toShadowMapMatrix);
		m_waterRenderer->SetShadowMatrix(m_toShadowMapMatrix);
	}

	Layer3D::~Layer3D()
	{
		delete m_entityRenderer;
		delete m_terrainRenderer;
		delete m_selectionRenderer;
		delete m_skyboxRenderer;
		delete m_waterRenderer;
		delete m_normalEntityRenderer;
		delete m_shadowMapController;
		delete m_postProcessor;

		delete m_projectionMatrix;
		delete m_toShadowMapMatrix;

		delete m_waterBuffer;
	}

	void Layer3D::Init()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
		glEnable(GL_STENCIL_TEST);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	#if FULL_SELECTION
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	#else
		glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
	#endif
		glClearColor(m_fogColor->GetR(), m_fogColor->GetG(), m_fogColor->GetB(), m_fogColor->GetA());
		//glEnable(GL_CLIP_DISTANCE0); - Causes issues with the skybox rendering, determine whether this is required
	}

	void Layer3D::OnAttach()
	{
		// Create Shaders
		size_t numOfLights = m_pointlights.size();
		m_entityRenderer->SetShader(new Graphics::EntityShader(numOfLights));
		m_normalEntityRenderer->SetShader(new Graphics::NormalEntityShader(numOfLights));
		m_terrainRenderer->SetShader(new Graphics::TerrainShader(numOfLights));
		m_waterRenderer->SetShader(new Graphics::WaterShader(numOfLights));

		// Pass new Scene information
		m_shadowMapController->SetCamera(m_mainCamera);
		m_viewMatrix = m_mainCamera->GetViewMatrix();

		m_skyboxRenderer->SetSkyBox(m_skybox);

		m_shadowMapController->SetDirectionalLight(m_mainLight);
		m_entityRenderer->AddLight(m_mainLight);
		m_terrainRenderer->AddLight(m_mainLight);
		m_normalEntityRenderer->AddLight(m_mainLight);
		m_waterRenderer->AddLight(m_mainLight);
	#if ASTRA_DEBUG
		Graphics::GizmoController::AddGizmo(m_mainLight->GetGizmo());
	#endif

		for (auto* terrain : m_terrains)
		{
			m_terrainRenderer->AddTerrain(terrain);
		}
		for (auto* tile : m_tiles)
		{
			// Determine better method of offseting clipping planes.
			m_reflectionClipPlane.w = -tile->GetTranslation().y + 0.5f;
			m_refractionClipPlane.w = tile->GetTranslation().y + 0.5f;

			m_waterRenderer->AddTile(tile);
		}

		for (auto* light : m_pointlights)
		{
			m_entityRenderer->AddLight(light);
			m_normalEntityRenderer->AddLight(light);
			m_terrainRenderer->AddLight(light);
			m_waterRenderer->AddLight(light);
		#if ASTRA_DEBUG
			Graphics::GizmoController::AddGizmo(light->GetGizmo());
		#endif
		}
		for (auto* system : m_particles)
		{
		#if ASTRA_DEBUG
			Graphics::GizmoController::AddGizmo(system->GetGizmo());
		#endif
		}

		// Update Projection Matrix
		auto [width, height] = Application::Get().GetWindow().GetSize();
		UpdateScreen(width, height);

		m_attached = true;
	}

	void Layer3D::OnDetach()
	{
		m_attached = false;
	}

	void Layer3D::OnUpdate(float delta)
	{
		if (!m_attached) { return; }

		m_shadowMapController->Render();

		for (auto* system : m_particles)
		{
			system->GenerateParticles(delta);
		}
		
		if (m_waterBuffer && m_mainCamera)
		{
			float distance = 2 * (m_mainCamera->GetTranslation().y - m_refractionClipPlane.w);

			// Reflection Rendering
			m_waterRenderer->BindFrameBuffer(m_waterBuffer->GetReflectionBuffer()->GetId(), 320, 180);
			m_mainCamera->InvertPitch(-distance); // Updates the view matrix
			Render(delta, m_viewMatrix->Inverse() * Math::Vec4::W_Axis, true, m_reflectionClipPlane);
			m_waterRenderer->UnbindFrameBuffer();

			// Refraction Rendering
			m_mainCamera->InvertPitch(distance); // Updates the view matrix
			m_waterRenderer->BindFrameBuffer(m_waterBuffer->GetRefractionBuffer()->GetId(), 1280, 720);
			Render(delta, m_viewMatrix->Inverse() * Math::Vec4::W_Axis, true, m_refractionClipPlane);
			m_waterRenderer->UnbindFrameBuffer();
		}
		
		PreRender(delta);
		Math::Vec4 inverseView = m_viewMatrix->Inverse() * Math::Vec4::W_Axis;
		Render(delta, inverseView, false); // Renders entities and to stencil buffer when selected
		PostRender();
	}

	void Layer3D::SetSelectionColor(const Graphics::Color& color)
	{
		m_selectionRenderer->SetSelectionColor(color);
	}

	void Layer3D::AddEntity(const Graphics::Entity* entity)
	{
		if (entity->material->IsNormalMapped() || entity->material->IsParallaxMapped())
		{
			EmplaceEntity(EntityType::NormalMapped, entity);
		}
		else
		{
			EmplaceEntity(EntityType::Default, entity);
		}

		if (entity->IsSelected())
		{
			EmplaceEntity(EntityType::Selected, entity);
		}
		m_shadowMapController->AddEntity(entity);
	}

	void Layer3D::UpdateScreen(unsigned int width, unsigned int height)
	{
		*m_projectionMatrix = Math::Mat4::Perspective(static_cast<float>(width), static_cast<float>(height), FieldOfView, NearPlane, FarPlane);
		m_terrainRenderer->UpdateProjectionMatrix(m_projectionMatrix);
		m_entityRenderer->UpdateProjectionMatrix(m_projectionMatrix);
		m_normalEntityRenderer->UpdateProjectionMatrix(m_projectionMatrix);
		m_selectionRenderer->UpdateProjectionMatrix(m_projectionMatrix);
		m_skyboxRenderer->UpdateProjectionMatrix(m_projectionMatrix);
		m_waterRenderer->UpdateProjectionMatrix(m_projectionMatrix);
		if (m_postProcessor) 
		{
			m_postProcessor->UpdateScreenRatio(width, height);
		}
		Graphics::ParticleController::UpdateProjectionMatrix(m_projectionMatrix);
	#if ASTRA_DEBUG
		Graphics::GizmoController::UpdateProjectionMatrix(m_projectionMatrix);
	#endif
	}

	void Layer3D::PreRender(float delta)
	{
		Graphics::ParticleController::Update(delta, m_mainCamera->GetTranslation());

		// Connect Post Processing Buffer
		if (m_postProcessor)
		{
			m_postProcessor->Attach();
		}

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, m_shadowMapController->GetShadowMap());

		*m_toShadowMapMatrix = m_shadowMapController->GetToShadowMapSpaceMatrix();
	}

	void Layer3D::Render(float delta, const Math::Vec4& inverseViewVector, bool waterPass, const Math::Vec4& clipPlane)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		m_terrainRenderer->Draw(delta, m_viewMatrix, inverseViewVector, clipPlane);

		if (!waterPass)
		{
			m_skyboxRenderer->Draw(delta, m_viewMatrix, NULL);
			m_waterRenderer->Draw(delta, m_viewMatrix, inverseViewVector);

			m_entityRenderer->Draw(delta, m_entities[EntityType::Default], m_viewMatrix, inverseViewVector, clipPlane);
			m_normalEntityRenderer->Draw(delta, m_entities[EntityType::NormalMapped], m_viewMatrix, inverseViewVector, clipPlane);
		}
		else
		{
			m_entityRenderer->Draw(delta, m_entities[EntityType::Default], m_viewMatrix, inverseViewVector, clipPlane);
			m_normalEntityRenderer->Draw(delta, m_entities[EntityType::NormalMapped], m_viewMatrix, inverseViewVector, clipPlane);
			m_skyboxRenderer->Draw(delta, m_viewMatrix, NULL);
		}
	}

	void Layer3D::PostRender()
	{
		// Renders outline based on stencil buffer
		glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		m_selectionRenderer->Draw(0, m_entities[EntityType::Selected], m_viewMatrix);

		Graphics::ParticleController::Render(m_viewMatrix);

		// Perform Post Processing Effects
		if (m_postProcessor)
		{
			m_postProcessor->Detach();
			m_postProcessor->Draw();
		}

	#if ASTRA_DEBUG
		Graphics::GizmoController::Render(m_viewMatrix);
	#endif
	}

	void Layer3D::EmplaceEntity(unsigned int listIndex, const Graphics::Entity* entity)
	{
		auto temp = m_entities[listIndex].find(entity->vertexArray->vaoId);
		if (temp != m_entities[listIndex].end())
		{
			temp->second.emplace_back(entity);
		}
		else
		{
			m_entities[listIndex][entity->vertexArray->vaoId] = std::vector<const Graphics::Entity*>();
			m_entities[listIndex][entity->vertexArray->vaoId].emplace_back(entity);
		}
	}

#if ASTRA_DEBUG
	void Layer3D::ToggleWireframeMode(unsigned char state)
	{
		if (state == 0)
		{
			m_entityRenderer->SetWireframe(false);
			m_normalEntityRenderer->SetWireframe(false);
			m_waterRenderer->SetWireframe(false);
			m_terrainRenderer->SetWireframe(false);
		}
		else if (state > 0)
		{
			m_entityRenderer->SetWireframe(true);
			m_normalEntityRenderer->SetWireframe(true);

			if (state > 1)
			{
				m_waterRenderer->SetWireframe(true);
				m_terrainRenderer->SetWireframe(true);
			}
		}
	}
#endif

	void Layer3D::SetMultisampling(unsigned int sampleSize)
	{
		if (sampleSize > 0)
		{
			if (!m_postProcessor)
			{
				m_postProcessor = new Graphics::PostProcessor();
				auto [width, height] = Application::Get().GetWindow().GetSize();
				m_postProcessor->UpdateScreenRatio(width, height);
			}
			else
			{
				m_postProcessor->SetMultisampling(sampleSize);
			}
		}
		else
		{
			if (m_postProcessor)
			{
				m_postProcessor->SetMultisampling(sampleSize);
				if (m_postProcessor->IsEmpty())
				{
					delete m_postProcessor;
					m_postProcessor = NULL;
				}
			}
		}
	}

	void Layer3D::SetBloom(bool enabled)
	{
		if (enabled)
		{
			if (!m_postProcessor)
			{
				m_postProcessor = new Graphics::PostProcessor();
				auto [width, height] = Application::Get().GetWindow().GetSize();
				m_postProcessor->UpdateScreenRatio(width, height);
			}
			else
			{
				m_postProcessor->SetBloomEffect(enabled);
			}
		}
		else
		{
			if (m_postProcessor)
			{
				m_postProcessor->SetBloomEffect(enabled);
				if (m_postProcessor->IsEmpty())
				{
					delete m_postProcessor;
					m_postProcessor = NULL;
				}
			}
		}
	}

	void Layer3D::SetHDR(bool enabled)
	{
		if (enabled)
		{
			if (!m_postProcessor)
			{
				m_postProcessor = new Graphics::PostProcessor();
				auto [width, height] = Application::Get().GetWindow().GetSize();
				m_postProcessor->UpdateScreenRatio(width, height);
			}
			else
			{
				m_postProcessor->SetHDR(enabled);
			}
		}
		else
		{
			if (m_postProcessor)
			{
				m_postProcessor->SetHDR(enabled);
				if (m_postProcessor->IsEmpty())
				{
					delete m_postProcessor;
					m_postProcessor = NULL;
				}
			}
		}
	}
}
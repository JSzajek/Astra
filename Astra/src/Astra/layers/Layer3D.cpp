#include "astra_pch.h"

#include "Layer3D.h"

#include <thread>

#include "Astra/Application.h"

#include "Astra/graphics/loaders/Loader.h"

#include "Astra/graphics/shaders/EntityShader.h"
#include "Astra/graphics/shaders/NormalEntityShader.h"
#include "Astra/graphics/shaders/TerrainShader.h"
#include "Astra/graphics/shaders/SkyboxShader.h"
#include "Astra/graphics/shaders/WaterShader.h"
#include "Astra/graphics/shaders/GizmoShader.h"

#include "Astra/graphics/entities/Model.h"

namespace Astra
{
	Layer3D::Layer3D()
		: m_reflectionClipPlane(Math::Vec4(0, 1, 0, 0)), m_refractionClipPlane(Math::Vec4(0, -1, 0, 0)),
			m_projectionMatrix(new Math::Mat4(1)), m_toShadowMapMatrix(new Math::Mat4(1)),
			m_fogColor(new Graphics::Color(0.5f, 0.6f, 0.6f, 1.0f)), 
			m_viewMatrix(NULL), m_mainCamera(NULL),
			m_postProcessor(NULL), m_waterBuffer(NULL)

	{
		Init();

		m_selectionRenderer = new Graphics::SelectionRenderer();
		m_shadowMapController = new Graphics::ShadowMapController(FieldOfView, NearPlane, FarPlane);

		m_skyboxRenderer = new Graphics::SkyboxRenderer(new Graphics::SkyboxShader(), m_fogColor);
		m_entityRenderer = new Graphics::Entity3dRenderer(m_fogColor);
		m_terrainRenderer = new Graphics::TerrainRenderer(m_fogColor);
		m_normalEntityRenderer = new Graphics::NormalEntity3dRenderer(m_fogColor);
		m_waterRenderer = new Graphics::WaterRenderer(NearPlane, FarPlane);

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

	Graphics::LayerEntity* Layer3D::Get(std::string name)
	{
		if (m_loaded.find(name) != m_loaded.end())
		{
			return m_loaded[name];
		}
		return NULL;
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
		size_t numOfLights = m_pointLights.size();
		m_entityRenderer->SetShader(new Graphics::EntityShader(numOfLights));
		m_normalEntityRenderer->SetShader(new Graphics::NormalEntityShader(numOfLights));
		m_terrainRenderer->SetShader(new Graphics::TerrainShader(numOfLights));
		m_waterRenderer->SetShader(new Graphics::WaterShader(numOfLights));

		// Pass new Scene information
		m_shadowMapController->SetCamera(m_mainCamera);
		m_viewMatrix = m_mainCamera->GetViewMatrix();

		m_skyboxRenderer->SetSkyBox(m_skybox);

		m_shadowMapController->SetDirectionalLight(&m_mainLight);
		m_entityRenderer->AddLight(&m_mainLight);
		m_terrainRenderer->AddLight(&m_mainLight);
		m_normalEntityRenderer->AddLight(&m_mainLight);
		m_waterRenderer->AddLight(&m_mainLight);
	#if ASTRA_DEBUG
		Graphics::GizmoController::AddGizmo(m_mainLight.GetGizmo());
	#endif

		for (const auto& tileDir : m_waterTiles)
		{
			// Determine better method of offseting clipping planes.
			m_reflectionClipPlane.w = -tileDir.second.GetTranslation().y + 0.5f;
			m_refractionClipPlane.w =  tileDir.second.GetTranslation().y + 0.5f;
		}

		for (unsigned int i = 0; i < m_pointLights.size(); ++i)
		{
			m_entityRenderer->AddLight(i, &m_pointLights[i]);

			m_normalEntityRenderer->AddLight(i, &m_pointLights[i]);
			m_terrainRenderer->AddLight(i, &m_pointLights[i]);
			m_waterRenderer->AddLight(i, &m_pointLights[i]);
		#if ASTRA_DEBUG
			Graphics::GizmoController::AddGizmo(m_pointLights[i].GetGizmo());
		#endif
		}
		for (const auto& system : m_particles)
		{
		#if ASTRA_DEBUG
			Graphics::GizmoController::AddGizmo(system.second.GetGizmo());
		#endif
		}

		// Update Projection Matrix
		auto [width, height] = Application::Get().GetWindow().GetSize();
		UpdateScreen(width, height);

		// TODO: Load from app settings
		SetMultisampling(Application::Get().GetWindow().IsMultisampling());
		SetHDR(Application::Get().GetWindow().IsHDR());
		SetBloom(Application::Get().GetWindow().IsBloom());
		SetReflections(Application::Get().GetWindow().IsReflecting());

		m_attached = true;
	}

	void Layer3D::OnDetach()
	{
		m_attached = false;
	}

	void Layer3D::LayerUpdateAnimations(float delta)
	{
		for (const auto& model : m_models)
		{
			if (auto* animator = model.second.GetAnimator())
			{
				animator->UpdateAnimation(delta);
			}
		}
	}

	void Layer3D::OnUpdate(float delta)
	{
		// With heap stored particles - ~43 fps

		if (!m_attached) { return; }

		std::thread animationWorker(&Layer3D::LayerUpdateAnimations, this, delta);
		for (const auto& system : m_particles) // Moving to thread causes weird issues?
		{
			system.second.GenerateParticles(delta);
		}

		animationWorker.join();
		m_shadowMapController->Render(m_modelCategories[Graphics::ModelType::ShadowCaster]);
		
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
		Render(delta, inverseView, false); // Renders entities 
		PostRender();
	}

	void Layer3D::SetSelectionColor(const Graphics::Color& color)
	{
		m_selectionRenderer->SetSelectionColor(color);
	}

	void Layer3D::AddModel(const Graphics::Model& model)
	{
		unsigned char flag = 0;

		flag |= BIT(Graphics::ModelType::ShadowCaster); // Add Check if it is a shadow caster?

		// Grab Components
		if (model.HasAnimator())
		{
			m_animators.emplace_back(model.GetAnimator());
		}
		
		flag |= model.HasNormals() ? BIT(Graphics::ModelType::NormalMapped) : BIT(Graphics::ModelType::Default);
		
		if (model.IsSelected())
			flag |= BIT(Graphics::ModelType::Selected);

		EmplaceModel(flag, model);
	}

	void Layer3D::AddParticleSystem(const Graphics::ParticleSystem& system)
	{
		auto uid = system.GetUID();
		m_particles[uid] = system;
		m_loaded[system.ToString()] = &m_particles[uid];
	}

	Graphics::Terrain* Layer3D::AddTerrain(const Graphics::Terrain& terrain)
	{
		auto uid = terrain.GetUID();
		auto name = terrain.ToString();

		m_terrains[uid] = terrain;
		m_loaded[name] = &m_terrains[uid];

		if (m_terrainCategories.find(uid) == m_terrainCategories.end())
		{
			m_terrainCategories[uid] = std::vector<const Graphics::Terrain*>();
		}
		m_terrainCategories[uid].push_back(&m_terrains[uid]);
		return &m_terrains[uid];
	}

	void Layer3D::AddWaterTile(const Graphics::WaterTile& tile) 
	{
		auto uid = tile.GetUID();
		m_waterTiles[uid] = tile;
		m_loaded[tile.ToString()] = &m_waterTiles[uid];
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_terrainRenderer->Draw(delta, m_terrainCategories, m_viewMatrix, inverseViewVector, clipPlane);

		if (!waterPass)
		{
			m_skyboxRenderer->Draw(delta, m_viewMatrix, NULL);

			m_entityRenderer->Draw(delta, m_modelCategories[Graphics::ModelType::Default], m_viewMatrix, inverseViewVector, clipPlane);
			m_normalEntityRenderer->Draw(delta, m_modelCategories[Graphics::ModelType::NormalMapped], m_viewMatrix, inverseViewVector, clipPlane);
			m_waterRenderer->Draw(delta, m_waterTiles, m_viewMatrix, inverseViewVector);
			
		}
		else
		{
			m_entityRenderer->Draw(delta, m_modelCategories[Graphics::ModelType::Default], m_viewMatrix, inverseViewVector, clipPlane);
			m_normalEntityRenderer->Draw(delta, m_modelCategories[Graphics::ModelType::NormalMapped], m_viewMatrix, inverseViewVector, clipPlane);
			m_skyboxRenderer->Draw(delta, m_viewMatrix, NULL);
		}
	}

	void Layer3D::PostRender()
	{
		// Renders outline based on stencil buffer
		if (m_modelCategories[Graphics::ModelType::Selected].size() > 0)
		{
			glClear(GL_STENCIL_BUFFER_BIT);
			m_selectionRenderer->Draw(0, m_modelCategories[Graphics::ModelType::Selected], m_viewMatrix);
		}
		else
		{
			glStencilFunc(GL_ALWAYS, 0, 0xFF);
			glStencilMask(0xFF);
		}

		// Render particles after thus on top - Investigate Better Order Placement and Cost
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

	void Layer3D::EmplaceModel(unsigned char flags, const Graphics::Model& model)
	{
		auto string = model.ToString();
		auto renderId = model.GetRenderID();
		auto uid = model.GetUID();
		m_models[uid] = model;
		m_loaded[string] = &m_models[uid];

		for (unsigned char cat = Graphics::ModelType::Default; cat < 4; ++cat)
		{
			if (flags & BIT(cat))
			{
				if (m_modelCategories[cat].find(renderId) != m_modelCategories[cat].end())
				{
					m_modelCategories[cat][renderId].push_back(&m_models[uid]);
				}
				else
				{
					m_modelCategories[cat][renderId] = std::vector<const Graphics::Model*>();
					m_modelCategories[cat][renderId].push_back(&m_models[uid]);
				}
			}
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

			if (m_waterBuffer)
			{
				Graphics::Loader::UpdateFrameBuffer(m_waterBuffer->GetReflectionBuffer(), DefaultReflectionWidth, DefaultReflectionHeight, enabled, false);
				Graphics::Loader::UpdateFrameBuffer(m_waterBuffer->GetRefractionBuffer(), DefaultRefractionWidth, DefaultRefractionHeight, enabled, false);
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

			if (m_waterBuffer)
			{
				Graphics::Loader::UpdateFrameBuffer(m_waterBuffer->GetReflectionBuffer(), DefaultReflectionWidth, DefaultReflectionHeight, enabled, false);
				Graphics::Loader::UpdateFrameBuffer(m_waterBuffer->GetRefractionBuffer(), DefaultRefractionWidth, DefaultRefractionHeight, enabled, false);
			}
		}
	}

	void Layer3D::SetReflections(bool enabled)
	{
		if (enabled && !m_waterBuffer)
		{
			m_waterBuffer = Graphics::Loader::LoadWaterFrameBuffer(DefaultReflectionWidth, DefaultReflectionHeight,
																	DefaultRefractionWidth, DefaultRefractionHeight);
		}
		else
		{
			if (m_waterBuffer)
			{
				delete m_waterBuffer;
				m_waterBuffer = NULL;
			}
		}
		m_waterRenderer->SetFrameBuffer(m_waterBuffer);
		m_waterRenderer->SetReflection(enabled);
	}
}
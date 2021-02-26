#pragma once

#include <vector>
#include <unordered_map>
#include <thread>

#include "Layer.h"

#include "Astra/graphics/Color.h"

#include "Astra/graphics/renderers/Entity3dRenderer.h"
#include "Astra/graphics/renderers/NormalEntity3dRenderer.h"
#include "Astra/graphics/renderers/SelectionRenderer.h"
#include "Astra/graphics/renderers/TerrainRenderer.h"
#include "Astra/graphics/renderers/SkyboxRenderer.h"
#include "Astra/graphics/renderers/WaterRenderer.h"

#include "Astra/graphics/LayerEntity.h"
#include "Astra/graphics/entities/Model.h"
#include "Astra/graphics/entities/PointLight.h"
#include "Astra/graphics/entities/DirectionalLight.h"
#include "Astra/graphics/entities/terrains/Terrain.h"
#include "Astra/graphics/entities/terrains/WaterTile.h"

#include "Astra/graphics/entities/animation/Animator.h"

#include "Astra/graphics/particles/ParticleController.h"
#include "Astra/graphics/particles/ParticleSystem.h"
#include "Astra/graphics/materials/SkyboxMaterial.h"

#include "Astra/graphics/post_processing/PostProcessor.h"
#include "Astra/graphics/shadows/ShadowMapController.h"

#include "Astra/graphics/gizmos/GizmoController.h"

namespace Astra
{
	#define FULL_SELECTION		0

	class Layer3D : public Layer
	{
	private:
		static const constexpr unsigned int DefaultReflectionWidth = 320;
		static const constexpr unsigned int DefaultReflectionHeight = 180;

		static const constexpr unsigned int DefaultRefractionWidth = 1280;
		static const constexpr unsigned int DefaultRefractionHeight = 720;

		static const constexpr unsigned int FieldOfView = 70;
		static const constexpr float NearPlane = 0.1f;
		static const constexpr float FarPlane = 500.0f;
	private:
		std::mutex m_modelLock;
		std::mutex m_particleLock;

		// Renderers
		Graphics::Entity3dRenderer* m_entityRenderer;
		Graphics::NormalEntity3dRenderer* m_normalEntityRenderer;
		Graphics::SelectionRenderer* m_selectionRenderer;
		Graphics::TerrainRenderer* m_terrainRenderer;
		Graphics::SkyboxRenderer* m_skyboxRenderer;
		Graphics::WaterRenderer* m_waterRenderer;
		Graphics::PostProcessor* m_postProcessor;
		Graphics::ShadowMapController* m_shadowMapController;

		Graphics::Camera* m_mainCamera;
		const Math::Mat4* m_viewMatrix;

		Graphics::WaterFrameBuffer* m_waterBuffer;
		Math::Vec4 m_reflectionClipPlane;
		Math::Vec4 m_refractionClipPlane;

		Math::Mat4* m_projectionMatrix;
		Math::Mat4* m_toShadowMapMatrix;

		//Stored References
		std::unordered_map<std::string, Graphics::LayerEntity*> m_loaded;
		
		std::unordered_map<unsigned int, Graphics::Model> m_models;
		std::unordered_map<unsigned int, std::vector<const Graphics::Model*>> m_modelCategories[4];
		std::vector<Asset<Graphics::Animator>> m_animators;

		// Have an unordered_map that stores based off of name (which is unique).
		// Upon adding or removing from the map store in another multiset the names that correspond with certain categories
		// Update a list of references after adding or removing
	private:
		// Terrains
		std::unordered_map<unsigned int, Graphics::Terrain> m_terrains;
		std::unordered_map<unsigned int, std::vector<const Graphics::Terrain*>> m_terrainCategories;
		std::unordered_map<unsigned int, Graphics::WaterTile> m_waterTiles;
		std::vector<Graphics::WaterTile*> m_tiles;

		// Entities - 1: regular, 2: normal-mapped, 3: selected, 4: shadow-caster
		//std::unordered_map<unsigned int, std::vector<Graphics::Model>> m_models[4];
		std::unordered_map<unsigned int, Graphics::ParticleSystem> m_particles;

		// Lighting
		Graphics::DirectionalLight m_mainLight;
		std::vector<Graphics::PointLight> m_pointLights;
		Graphics::Color* m_fogColor;

		// Skybox
		Graphics::SkyboxMaterial m_skybox;

		// Gizmos
	#if ASTRA_DEBUG
		std::unordered_map<unsigned int, Graphics::Gizmo> m_gizmos;
		std::unordered_map<unsigned int, std::vector<const Graphics::Gizmo*>> m_gizmoCategories;
	#endif
		bool m_attached = false;
	public:
		Layer3D();
		~Layer3D();
	public:
		Graphics::LayerEntity* Get(std::string name);

		inline void SetDirectionalLight(const Graphics::DirectionalLight& light) { m_mainLight = light; }
		inline void SetSkyBox(const Graphics::SkyboxMaterial& material) { m_skybox = material; }
		inline void SetFogColor(const Graphics::Color& color) { *m_fogColor = color; }
		inline void SetCamera(Graphics::Camera* camera) { m_mainCamera = camera; }
		void SetSelectionColor(const Graphics::Color& color);

		Graphics::Model* AddModel(const Graphics::Model& model);
		void AddParticleSystem(const Graphics::ParticleSystem& system);
		Graphics::Terrain* AddTerrain(const Graphics::Terrain& terrain);
		void AddWaterTile(const Graphics::WaterTile& tile);
		inline void AddPointLight(const Graphics::PointLight& light) { m_pointLights.emplace_back(light); }
		
		void SetMultisampling(unsigned int sampleSize);
		void SetBloom(bool enabled);
		void SetHDR(bool enabled);
		void SetReflections(bool enabled);
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float delta) override;
		virtual void UpdateScreen(unsigned int width, unsigned int height) override;
	public:
	#if ASTRA_DEBUG
		void ToggleWireframeMode(unsigned char state);
	#endif
	private:
	#if ASTRA_DEBUG
		void AddGizmo(const Graphics::Gizmo& gizmo);
	#endif
		void LayerUpdateAnimations(float delta);
		//void LayerUpdateParticles(float delta);
	private:
		void Init();
		void PreRender(float delta);
		void Render(float delta, const Math::Vec4& inverseViewVector, bool waterPass, 
					const Math::Vec4& clipPlane = Graphics::Renderer::DefaultClipPlane);
		void PostRender();
		Graphics::Model* EmplaceModel(unsigned char flags, const Graphics::Model& model);
	};
}
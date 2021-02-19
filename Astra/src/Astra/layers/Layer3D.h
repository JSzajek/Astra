#pragma once

#include <vector>
#include <unordered_map>

#include "Layer.h"

#include "Astra/graphics/Color.h"

#include "Astra/graphics/renderers/Entity3dRenderer.h"
#include "Astra/graphics/renderers/NormalEntity3dRenderer.h"
#include "Astra/graphics/renderers/SelectionRenderer.h"
#include "Astra/graphics/renderers/TerrainRenderer.h"
#include "Astra/graphics/renderers/SkyboxRenderer.h"
#include "Astra/graphics/renderers/WaterRenderer.h"

#include "Astra/graphics/entities/Entity.h"
#include "Astra/graphics/entities/PointLight.h"
#include "Astra/graphics/entities/DirectionalLight.h"
#include "Astra/graphics/entities/terrains/Terrain.h"
#include "Astra/graphics/entities/terrains/WaterTile.h"

#include "Astra/graphics/particles/ParticleController.h"
#include "Astra/graphics/particles/ParticleSystem.h"
#include "Astra/graphics/materials/SkyboxMaterial.h"

#include "Astra/graphics/post_processing/PostProcessor.h"
#include "Astra/graphics/shadows/ShadowMapController.h"

#include "Astra/graphics/gizmos/GizmoController.h"

namespace Astra
{
	#define FULL_SELECTION		1

	enum EntityType : unsigned char
	{
		Default, NormalMapped, Selected
	};

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
	private:
		// Terrains
		std::vector<const Graphics::Terrain*> m_terrains;
		std::vector<const Graphics::WaterTile*> m_tiles;

		// Entities - 1: regular, 2: normal-mapped, 3: selected
		std::unordered_map<unsigned int, std::vector<const Graphics::Entity*>> m_entities[3];
		std::vector<const Graphics::ParticleSystem*> m_particles;

		// Lighting
		std::vector<Graphics::PointLight*> m_pointlights;
		Graphics::DirectionalLight* m_mainLight;
		Graphics::Color* m_fogColor;

		// Skybox
		const Graphics::SkyboxMaterial* m_skybox;

		bool m_attached = false;
	public:
		Layer3D();
		~Layer3D();
	public:
		inline void SetDirectionalLight(Graphics::DirectionalLight* light) { m_mainLight = light; }
		inline void SetSkyBox(const Graphics::SkyboxMaterial* material) { m_skybox = material; }
		inline void SetFogColor(const Graphics::Color& color) { *m_fogColor = color; }
		inline void SetCamera(Graphics::Camera* camera) { m_mainCamera = camera; }
		void SetSelectionColor(const Graphics::Color& color);

		void AddEntity(const Graphics::Entity* entity);
		inline void AddParticleSystem(const Graphics::ParticleSystem* system) { m_particles.emplace_back(system); }
		inline void AddTerrain(const Graphics::Terrain* terrain) { m_terrains.emplace_back(terrain); }
		inline void AddWaterTile(const Graphics::WaterTile* tile) { m_tiles.emplace_back(tile); }
		inline void AddPointLight(Graphics::PointLight* light) { m_pointlights.emplace_back(light); }
		
		void SetMultisampling(unsigned int sampleSize);
		void SetPostProcessing(bool enabled);
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
		void Init();
		void PreRender(float delta);
		void Render(float delta, const Math::Vec4& inverseViewVector, bool waterPass, const Math::Vec4& clipPlane = Graphics::Renderer::DefaultClipPlane);
		void PostRender();
		void EmplaceEntity(unsigned int listIndex, const Graphics::Entity* entity);
	};
}
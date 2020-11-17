#pragma once

#include <vector>
#include <unordered_map>

#include "../../math/Maths.h"

#include "../entities/DirectionalLight.h"
#include "../entities/PointLight.h"
#include "../entities/SpotLight.h"
#include "../entities/Camera.h"
#include "../entities/terrains/WaterTile.h"
#include "../entities/Entity.h"
#include "../textures/GuiTexture.h"
#include "../particles/ParticleSystem.h"
#include "../fonts/GuiText.h"
#include "../materials/SkyboxMaterial.h"
#include "../entities/terrains/Terrain.h"

namespace Astra::Graphics
{
	class Scene
	{
	private:
		bool m_editing;

		Camera* m_mainCamera;
		Math::Vec3 m_fogColor;
		DirectionalLight* m_mainLight;
		const SkyboxMaterial* m_skybox;
		const SpotLight* m_spotLight;

		std::vector<const Entity*> m_entities;
		std::vector<const ParticleSystem*> m_particles;
		std::vector<GuiTexture*> m_guis;
		std::vector<GuiText*> m_texts;
		std::vector<PointLight*> m_pointlights;
		//std::vector<const SpotLight*> m_spotlights;
		std::vector<const Terrain*> m_terrains;
		std::vector<const WaterTile*> m_tiles;
	public:
		Scene(Camera* main, const Math::Vec3& fogColor = Math::Vec3(0.5f, 0.6f, 0.6f))
			: m_editing(false), m_mainCamera(main), m_fogColor(fogColor), m_skybox(NULL), m_mainLight(NULL), m_spotLight(NULL)
		{
		}

		~Scene()
		{
		}

		inline void Start()
		{
			// Start Scene Creation
			m_editing = true;
		}

		inline void End()
		{
			// End Scene Creation and Push to Render Loop
			m_editing = false;
		}

		inline void SetMainCamera(Camera* camera) { m_mainCamera = camera; }
		inline void SetDirectionalLight(DirectionalLight* light) { m_mainLight = light; }
		inline void SetSkyBox(const SkyboxMaterial* material) { m_skybox = material; }
		inline void SetFogColor(const Math::Vec3& color) { m_fogColor = color; }

		inline void AddEntity(const Entity* entity) { m_entities.push_back(entity); }
		inline void AddParticleSystem(const ParticleSystem* system) { m_particles.push_back(system); }
		inline void AddGui(GuiTexture* gui) { m_guis.push_back(gui); }
		inline void AddText(GuiText* text) { m_texts.push_back(text); }
		inline void AddTerrain(const Terrain* terrain) { m_terrains.push_back(terrain); }
		inline void AddWaterTile(const WaterTile* tile) { m_tiles.push_back(tile); }
		
		inline DirectionalLight* const GetDirectionalLight() const { return m_mainLight; }
		inline const Math::Vec3& GetFogColor() const { return m_fogColor; }
		inline const SkyboxMaterial* const GetSkyBox() const { return m_skybox; }

		inline Camera* const GetCamera() const { return m_mainCamera; }
		inline const std::vector<const Entity*>& GetEntities() const { return m_entities; }
		inline const std::vector<PointLight*>& GetPointLights() const { return m_pointlights; }
		inline const std::vector<const Terrain*>& GetTerrains() const { return m_terrains; }
		inline const std::vector<GuiTexture*>& GetGuis() const { return m_guis; }
		inline const std::vector<GuiText*>& GetTexts() const { return m_texts; }
		inline const std::vector<const WaterTile*>& GetWaterTiles() const { return m_tiles; }
		inline const std::vector<const ParticleSystem*>& GetParticles() const { return m_particles; }
		
		inline void AddPointLight(PointLight* light) { m_pointlights.push_back(light); }
		inline void AddSpotLight(const SpotLight* light) { m_spotLight = light; }
	};
}
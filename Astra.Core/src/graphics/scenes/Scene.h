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
		const DirectionalLight* m_mainLight;
		const SkyboxMaterial* m_skybox;
		const SpotLight* m_spotLight;

		std::vector<const Entity*> m_entities;
		std::vector<const ParticleSystem*> m_particles;
		std::vector<const GuiTexture*> m_guis;
		std::vector<const GuiText*> m_texts;
		std::vector<const PointLight*> m_pointlights;
		//std::vector<const SpotLight*> m_spotlights;
		std::vector<const Terrain*> m_terrains;
		std::vector<const WaterTile*> m_tiles;
	public:
		Scene(Camera* main, const Math::Vec3& fogColor = Math::Vec3(0.5f, 0.6f, 0.6f));
		~Scene();

		void Start();
		void End();

		inline void SetMainCamera(Camera* camera) { m_mainCamera = camera; }
		inline void SetDirectionalLight(const DirectionalLight* light) { m_mainLight = light; }
		inline void SetSkyBox(const SkyboxMaterial* material) { m_skybox = material; }
		inline void SetFogColor(const Math::Vec3& color) { m_fogColor = color; }

		inline void AddEntity(const Entity* entity) { m_entities.push_back(entity); }
		inline void AddParticleSystem(const ParticleSystem* system) { m_particles.push_back(system); }
		inline void AddGui(const GuiTexture* gui) { m_guis.push_back(gui); }
		inline void AddText(const GuiText* text) { m_texts.push_back(text); }
		inline void AddTerrain(const Terrain* terrain) { m_terrains.push_back(terrain); }
		inline void AddWaterTile(const WaterTile* tile) { m_tiles.push_back(tile); }
		
		inline const DirectionalLight* const GetDirectionalLight() const { return m_mainLight; }
		inline const Math::Vec3& const GetFogColor() const { return m_fogColor; }

		inline Camera* const GetCamera() const { return m_mainCamera; }
		inline const std::vector<const Entity*>& const GetEntities() const { return m_entities; }
		inline const std::vector<const PointLight*>& const GetPointLights() const { return m_pointlights; }
		inline const std::vector<const Terrain*>& const GetTerrains() const { return m_terrains; }
		inline const std::vector<const GuiTexture*>& const GetGuis() const { return m_guis; }
		
		void AddPointLight(const PointLight* light);
		void AddSpotLight(const SpotLight* light);
	};
}
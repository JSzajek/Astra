#pragma once

#include <map>
#include <vector>

#include "GuiRenderer.h"
#include "Entity3dRenderer.h"
#include "TerrainRenderer.h"
#include "SkyboxRenderer.h"
#include "../shaders/GuiShader.h"
#include "../shaders/BasicShader.h"
#include "../shaders/LightingShader.h"
#include "../shaders/TerrainShader.h"
#include "../shaders/SkyboxShader.h"
#include "../textures/GuiTexture.h"
#include "../entities/Camera.h"
#include "../../math/Mat4Utils.h"

#include "../../logger/Logger.h"

namespace Astra::Graphics
{
	class RendererController
	{
	private:
		const unsigned int FieldOfView = 70;
		const float NearPlane = 0.1f;
		const float FarPlane = 500.0f;
	private:
		GuiShader* m_guiShader;
		GuiRenderer* m_guiRenderer;
		BasicShader* m_basicShader;
		LightingShader* m_lightingShader;
		TerrainShader* m_terrainShader;
		SkyboxShader* m_skyboxShader;
		Entity3dRenderer* m_entityRenderer;
		TerrainRenderer* m_terrainRenderer;
		SkyboxRenderer* m_skyboxRenderer;
		Camera* m_mainCamera;

		Math::Vec3 skyColor;

		Math::Mat4 projectionMatrix;
		Math::Mat4 viewMatrix;
		Math::Mat4 modelViewMatrix;

	public:
		RendererController(const Math::Vec3& skyColor = Math::Vec3(0.5f, 0.6f, 0.6f));
		~RendererController();
		void Init() const;
		void UpdateScreen(float width, float height);
		void UpdateCameraView();
		void Render();

		inline void AddGui(const GuiTexture* gui) { m_guiRenderer->AddGui(gui); }
		inline void AddEntity(const Entity* entity) { m_entityRenderer->AddEntity(entity); }
		inline void AddTerrain(const Terrain* terrain) { m_terrainRenderer->AddTerrain(terrain); }
		
		inline void AddLight(Light* light) 
		{ 
			m_entityRenderer->AddLight(light);
			m_terrainRenderer->AddLight(light);
		}
		
		inline void SetMainCamera(Camera* camera) { m_mainCamera = camera; }
	};
}
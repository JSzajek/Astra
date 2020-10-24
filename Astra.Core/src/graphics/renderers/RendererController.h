#pragma once

#include <map>
#include <vector>

#include "GuiRenderer.h"
#include "Entity3dRenderer.h"
#include "TerrainRenderer.h"
#include "SkyboxRenderer.h"
#include "WaterRenderer.h"
#include "NormalEntity3dRenderer.h"
#include "../fonts/FontController.h"
#include "../shaders/GuiShader.h"
#include "../shaders/BasicShader.h"
#include "../shaders/LightingShader.h"
#include "../shaders/TerrainShader.h"
#include "../shaders/SkyboxShader.h"
#include "../shaders/WaterShader.h"
#include "../shaders/NormalEntityShader.h"
#include "../textures/GuiTexture.h"
#include "../entities/Camera.h"
#include "../../math/Mat4Utils.h"

#include "../buffers/WaterFrameBuffer.h"

#include "../../logger/Logger.h"

namespace Astra::Graphics
{
	class RendererController
	{
	private:
		static const constexpr float DefaultReflectionWidth = 320;
		static const constexpr float DefaultReflectionHeight = 180;

		static const constexpr float DefaultRefractionWidth = 1280;
		static const constexpr float DefaultRefractionHeight = 720;

		static const constexpr unsigned int FieldOfView = 70;
		static const constexpr float NearPlane = 0.1f;
		static const constexpr float FarPlane = 500.0f;
	private:
		GuiShader* m_guiShader;
		GuiRenderer* m_guiRenderer;
		LightingShader* m_lightingShader;
		TerrainShader* m_terrainShader;
		SkyboxShader* m_skyboxShader;
		WaterShader* m_waterShader;
		NormalEntityShader* m_normalEntityShader;

		Entity3dRenderer* m_entityRenderer;
		TerrainRenderer* m_terrainRenderer;
		SkyboxRenderer* m_skyboxRenderer;
		WaterRenderer* m_waterRenderer;
		NormalEntity3dRenderer* m_normalEntityRenderer;

		Camera* m_mainCamera;

		WaterFrameBuffer* m_waterBuffer;
		Math::Vec4 m_reflectionClipPlane;
		Math::Vec4 m_refractionClipPlane;

		Math::Vec3 fogColor;

		Math::Mat4 projectionMatrix;
		Math::Mat4 viewMatrix;
		Math::Mat4 modelViewMatrix;

	public:
		RendererController(const Math::Vec3& fogColor = Math::Vec3(0.5f, 0.6f, 0.6f));
		~RendererController();
		void Init() const;
		void UpdateScreen(float width, float height);
		void UpdateCameraView();
		void Render();
		void PreRender(const Math::Vec4& clipPlane = Renderer::DefaultClipPlane);
		void PostRender();
		void GuiRender();

		inline void AddGui(const GuiTexture* gui) { m_guiRenderer->AddGui(gui); }
		inline void AddText(GuiText* text) { FontController::LoadText(*text); }
		inline void AddEntity(const Entity* entity) 
		{ 
			if (entity->IsNormalMapped())
			{
				m_normalEntityRenderer->AddEntity(entity);
			}
			else
			{
				m_entityRenderer->AddEntity(entity); 
			}
		}

		inline void AddTerrain(const Terrain* terrain) { m_terrainRenderer->AddTerrain(terrain); }
		
		inline void AddLight(Light* light) 
		{
			m_terrainRenderer->AddLight(light);
			m_entityRenderer->AddLight(light);
			m_normalEntityRenderer->AddLight(light);
			m_waterRenderer->AddLight(light);
		}

		inline void AddWaterTile(const WaterTile& tile) 
		{
			m_reflectionClipPlane.w = -tile.GetTranslation().y + 1.6f;
			m_refractionClipPlane.w = tile.GetTranslation().y + 1.6f;
			m_waterRenderer->AddTile(tile); 
		}

		inline void SetMainCamera(Camera* camera) 
		{ 
			m_mainCamera = camera;
			m_waterRenderer->SetCamera(camera);
		}

		inline void SetSkyBox(const SkyboxMaterial* material) { m_skyboxRenderer->SetSkyBox(material); }
	};
}
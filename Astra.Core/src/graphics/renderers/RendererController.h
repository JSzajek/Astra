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
#include "../particles/ParticleController.h"
#include "../gizmos/GizmoController.h"
#include "../shadows/ShadowMapController.h"
#include "../shaders/GuiShader.h"
#include "../shaders/EntityShader.h"
#include "../shaders/TerrainShader.h"
#include "../shaders/SkyboxShader.h"
#include "../shaders/WaterShader.h"
#include "../shaders/NormalEntityShader.h"
#include "../textures/GuiTexture.h"
#include "../entities/Camera.h"
#include "../../math/Mat4Utils.h"

#include "../buffers/WaterFrameBuffer.h"

#include "../scenes/Scene.h"

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
		Entity3dRenderer* m_entityRenderer;
		TerrainRenderer* m_terrainRenderer;
		SkyboxRenderer* m_skyboxRenderer;
		WaterRenderer* m_waterRenderer;
		NormalEntity3dRenderer* m_normalEntityRenderer;
		GuiRenderer* m_guiRenderer;
		ShadowMapController* m_shadowMapController;

		Camera* m_mainCamera;

		WaterFrameBuffer* m_waterBuffer;
		Math::Vec4 m_reflectionClipPlane;
		Math::Vec4 m_refractionClipPlane;

		std::vector<const ParticleSystem*> m_systems;

		Math::Vec3* m_fogColor;
		Math::Mat4 projectionMatrix;
		Math::Mat4 viewMatrix;
		Math::Mat4 modelViewMatrix;

		const Scene* m_currentScene;
		bool m_block;
	public:
		RendererController(const RendererController&) = delete;
		void operator=(const RendererController&) = delete;

		static RendererController& Get()
		{
			static RendererController instance;
			return instance;
		}

		static void Render()
		{
			Get().RenderImpl();
		}

		static void UpdateScreen(float width, float height)
		{
			Get().UpdateScreenImpl(width, height);
		}

		static bool SetCurrentScene(const Scene* scene)
		{
			return Get().SetCurrentSceneImpl(scene);
		}

	private:
		RendererController();
		~RendererController();
		void UpdateScreenImpl(float width, float height);
		void RenderImpl();
		
		void Init() const;
		void UpdateCameraView();
		bool SetCurrentSceneImpl(const Scene* scene);
		void Clear();

		void PrepareRender();
		void PreRender(const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane = Renderer::DefaultClipPlane);
		void PostRender(const Math::Vec4& inverseViewVector);
		void GuiRender();
	};
}
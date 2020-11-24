#pragma once

#include <map>
#include <vector>

#include "GuiRenderer.h"
#include "Entity3dRenderer.h"
#include "TerrainRenderer.h"
#include "SkyboxRenderer.h"
#include "WaterRenderer.h"
#include "NormalEntity3dRenderer.h"
#include "../post_processing/PostProcessor.h"
#include "../fonts/FontController.h"
#include "../particles/ParticleController.h"
#include "../gizmos/GizmoController.h"
#include "../shadows/ShadowMapController.h"
#include "../shaders/GuiShader.h"
#include "../shaders/ContrastShader.h"
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
#define FULL_SELECTION 0

	class RendererController
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
		Entity3dRenderer* m_entityRenderer;
		TerrainRenderer* m_terrainRenderer;
		SkyboxRenderer* m_skyboxRenderer;
		WaterRenderer* m_waterRenderer;
		NormalEntity3dRenderer* m_normalEntityRenderer;
		GuiRenderer* m_guiRenderer;
		ShadowMapController* m_shadowMapController;

		PostProcessor* m_postProcessor;

		Camera* m_mainCamera;
		const Math::Mat4* viewMatrix;

		WaterFrameBuffer* m_waterBuffer;
		Math::Vec4 m_reflectionClipPlane;
		Math::Vec4 m_refractionClipPlane;

		std::vector<const ParticleSystem*> m_systems;

		Math::Vec3* m_fogColor;
		Math::Mat4* projectionMatrix;
		Math::Mat4* m_toShadowMapMatrix;

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

		static void Render(float delta)
		{
			Get().RenderImpl(delta);
		}

		static void UpdateScreen(int width, int height)
		{
			Get().UpdateScreenImpl(width, height);
		}

		static bool SetCurrentScene(const Scene* scene)
		{
			return Get().SetCurrentSceneImpl(scene);
		}

		static void SetSelectionColor(const Math::Vec3& color)
		{
			Get().SetSelectionColorImpl(color);
		}

	private:
		RendererController();
		~RendererController();
		void UpdateScreenImpl(int width, int height);
		void RenderImpl(float delta);

		void Init() const;
		bool SetCurrentSceneImpl(const Scene* scene);
		void SetSelectionColorImpl(const Math::Vec3& color);
		void Clear();

		void PrepareRender(float delta);
		void PreRender(float delta, const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane = Renderer::DefaultClipPlane);
		void PostRender(const Math::Vec4& inverseViewVector);
		void GuiRender();
	};
}
#pragma once

#include <map>
#include <vector>
#include <tuple>

#include "GuiRenderer.h"
#include "Entity3dRenderer.h"
#include "TerrainRenderer.h"
#include "SkyboxRenderer.h"
#include "WaterRenderer.h"
#include "NormalEntity3dRenderer.h"
#include "../post_processing/PostProcessor.h"
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
#include "../guis/Gui.h"
#include "../entities/Camera.h"
#include "../../math/Mat4Utils.h"

#include "../buffers/WaterFrameBuffer.h"
#include "../scenes/Scene.h"

namespace Astra::Graphics
{
	#define FULL_SELECTION		0

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
		Gui* m_last;
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

		static void CheckGuis()
		{
			Get().CheckGuisImpl();
		}

		static void CheckInput(const Math::Vec2& position, int action)
		{
			Get().CheckInputImpl(position, action);
		}

	#if _DEBUG
		static void ToggleWireframeMode()
		{
			static unsigned char wireframe = 0;
			Get().ToggleWireframeModeImpl(wireframe);
			wireframe = (wireframe + 1) % 3;
		}
	#endif

	private:
		RendererController();
		~RendererController();
		void UpdateScreenImpl(int width, int height);
		void RenderImpl(float delta);

		void CheckGuisImpl();
		void CheckInputImpl(const Math::Vec2& position, int action);

		void Init() const;
		bool SetCurrentSceneImpl(const Scene* scene);
		void SetSelectionColorImpl(const Math::Vec3& color);
		void Clear();

		void PrepareRender(float delta);
		void Render(float delta, const Math::Vec4& inverseViewVector, bool waterPass, const Math::Vec4& clipPlane = Renderer::DefaultClipPlane);
		void GuiRender();
	#if _DEBUG
		void ToggleWireframeModeImpl(unsigned char state);
	#endif
	};
}
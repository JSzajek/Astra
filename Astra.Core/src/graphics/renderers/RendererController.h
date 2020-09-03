#pragma once

#include <map>
#include <vector>

#include "GuiRenderer.h"
#include "Entity3dRenderer.h"
#include "../shaders/GuiShader.h"
#include "../shaders/BasicShader.h"
#include "../shaders/LightingShader.h"
#include "../textures/GuiTexture.h"
#include "../entities/Camera.h"
#include "../../math/Mat4Utils.h"

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
		Entity3dRenderer* m_entityRenderer;
		Camera m_camera;
		Light m_light;

		Math::Mat4 projectionMatrix;
		Math::Mat4 viewMatrix;
		Math::Mat4 modelViewMatrix;

	public:
		RendererController();
		~RendererController();
		void UpdateScreen(float width, float height);
		void Render();
		void AddGui(const GuiTexture& texture);
		void AddEntity(const Entity& entity);

		void UpdatePitch(float value);
		void UpdateYaw(float value);
		void UpdateDistance(float value);
		
	};
}
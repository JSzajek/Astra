#include "RendererController.h"

namespace Astra::Graphics
{
	RendererController::RendererController()
		: m_camera(Camera()), m_light(Light(Math::Vec3(-5,5,0), Math::Vec3(1, 1, 1)))
	{
		m_guiShader = new GuiShader();
		m_guiRenderer = new GuiRenderer(m_guiShader);

		m_basicShader = new BasicShader();
		m_lightingShader = new LightingShader();
		m_entityRenderer = new Entity3dRenderer(m_lightingShader);

		modelViewMatrix = Math::Mat4::Identity();
	
		m_entityRenderer->AddLight(m_light);
	}

	RendererController::~RendererController()
	{
		delete m_guiRenderer;
		delete m_entityRenderer;
	}

	void RendererController::UpdateScreen(float width, float height)
	{
		projectionMatrix = Math::Mat4::Perspective(width, height, FieldOfView, NearPlane, FarPlane);
		m_entityRenderer->UpdateProjectionMatrix(projectionMatrix);
	}

	void RendererController::Render()
	{
		m_camera.UpdatePosition();
		viewMatrix = Math::Mat4Utils::ViewMatrix(m_camera);

		m_entityRenderer->Draw(viewMatrix);
		m_guiRenderer->Draw(NULL);
	}

	void RendererController::AddGui(const GuiTexture* gui)
	{
		m_guiRenderer->AddGui(gui);
	}

	void RendererController::AddEntity(const Entity* entity)
	{
		m_entityRenderer->AddEntity(entity);
	}

	void RendererController::UpdatePitch(float value)
	{
		m_camera.SetPitch(m_camera.GetPitch() + value);
	}

	void RendererController::UpdateYaw(float value)
	{
		m_camera.SetSwivel(m_camera.GetSwivel() + value);
	}

	void RendererController::UpdateDistance(float value)
	{
		m_camera.SetDistance(m_camera.GetDistance() + value);
	}
}
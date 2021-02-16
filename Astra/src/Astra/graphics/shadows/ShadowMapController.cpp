#include "astra_pch.h"

#include "ShadowMapController.h"

#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	ShadowMapController::ShadowMapController(float fov, float _near, float _far)
		: m_projectionMatrix(1), m_lightViewMatrix(1), m_projectionViewMatrix(1), m_offset(CreateOffset())
	{
		m_shader = new ShadowShader();
		m_box = new ShadowBox(fov, _near, _far);
		m_buffer = Loader::LoadShadowFrameBuffer(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
		m_renderer = new ShadowMapRenderer(m_shader, m_buffer, m_projectionViewMatrix);
	}

	ShadowMapController::~ShadowMapController()
	{
		delete m_box;
		delete m_buffer;
		delete m_renderer;
	}

	void ShadowMapController::SetDirectionalLight(const Light* light) 
	{ 
		m_light = light;
		UpdateLightViewMatrix(m_light->GetRotation() * -1);
	}

	void ShadowMapController::Render()
	{
		if (m_box->Update() && m_light != NULL)
		{
			Prepare();
			m_renderer->Draw();
		}
	}

	void ShadowMapController::Prepare()
	{
		UpdateOrthoProjectionMatrix(m_box->GetWidth(), m_box->GetHeight(), m_box->GetLength());
		m_projectionViewMatrix = m_projectionMatrix * m_lightViewMatrix;
		m_renderer->SetProjectionViewMatrix(m_projectionViewMatrix);
	}
	
	void ShadowMapController::UpdateLightViewMatrix(Math::Vec3 direction)
	{
		direction.Normalize();
		m_lightViewMatrix.SetIdentity();
		float pitch = Math::ToDegrees(acosf(Math::Vec2(direction.x, direction.z).Length()));
		m_lightViewMatrix.Rotate(pitch, Math::Vec3::X_Axis);
		float yaw = Math::ToDegrees(atanf(direction.x / direction.z));
		yaw = direction.z > 0 ? yaw - 180 : yaw;
		m_lightViewMatrix.Rotate(-yaw, Math::Vec3::Y_Axis);
	}

	void ShadowMapController::UpdateOrthoProjectionMatrix(float width, float height, float length)
	{
		m_projectionMatrix.SetIdentity();
		m_projectionMatrix.columns[0][0] = 2.0f / width;
		m_projectionMatrix.columns[1][1] = 2.0f / height;
		m_projectionMatrix.columns[2][2] = -2.0f / length;
	}

	const Math::Mat4 ShadowMapController::CreateOffset()
	{
		Math::Mat4 offset(1);
		offset = offset.Translate(0.5f);
		offset = offset.Scale(0.5f);
		return offset;
	}
}
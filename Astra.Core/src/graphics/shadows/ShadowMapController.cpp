#include "ShadowMapController.h"

#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	ShadowMapController::ShadowMapController(Camera* camera, float fov, float near, float far)
		: m_projectionMatrix(1), m_lightViewMatrix(1), m_projectionViewMatrix(1), m_offset(CreateOffset())
	{
		m_shader = new ShadowShader();
		m_box = new ShadowBox(m_lightViewMatrix, camera, fov, near, far);
		m_buffer = Loader::LoadShadowFrameBuffer(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
		m_renderer = new ShadowMapRenderer(m_shader, m_buffer, m_projectionViewMatrix);
	}

	ShadowMapController::~ShadowMapController()
	{

	}

	void ShadowMapController::Render()
	{
		if (m_box->Update() && m_light != NULL)
		{
			Prepare(m_light->GetTranslation() * -1);
			m_renderer->Draw(NULL, NULL);
		}
	}

	void ShadowMapController::Prepare(Math::Vec3 lightDirection)
	{
		UpdateOrthoProjectionMatrix(m_box->GetWidth(), m_box->GetHeight(), m_box->GetLength());
		UpdateLightViewMatrix(lightDirection, m_box->GetCenter());
		m_projectionViewMatrix = m_projectionMatrix * m_lightViewMatrix;
		m_renderer->SetProjectionViewMatrix(m_projectionViewMatrix);
	}
	
	void ShadowMapController::UpdateLightViewMatrix(Math::Vec3 direction, Math::Vec3 center)
	{
		direction.Normalize();
		center *= -1;
		m_lightViewMatrix.SetIdentity();
		float pitch = Math::ToDegrees(acosf(Math::Vec2(direction.x, direction.z).Magnitude()));
		m_lightViewMatrix = m_lightViewMatrix.Rotate(pitch, Math::XAxis);
		float yaw = Math::ToDegrees(atanf(direction.x / direction.z));
		yaw = direction.z > 0 ? yaw - 180 : yaw;
		m_lightViewMatrix = m_lightViewMatrix.Rotate(-yaw, Math::YAxis);
		m_lightViewMatrix = m_lightViewMatrix.Translate(center);
	}

	void ShadowMapController::UpdateOrthoProjectionMatrix(float width, float height, float length)
	{
		m_projectionMatrix.SetIdentity();
		m_projectionMatrix.columns[0][0] = 2.0f / width;
		m_projectionMatrix.columns[1][1] = 2.0f / height;
		m_projectionMatrix.columns[2][2] = -2.0f / length;
	}

	const Math::Mat4& ShadowMapController::CreateOffset()
	{
		Math::Mat4 offset(1);
		offset = offset.Translate(0.5f);
		offset = offset.Scale(0.5f);
		return offset;
	}
}
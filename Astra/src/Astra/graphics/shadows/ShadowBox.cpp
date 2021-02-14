#include "astra_pch.h"

#include "ShadowBox.h"

#include "Astra/Application.h"

namespace Astra::Graphics
{
	ShadowBox::ShadowBox(float fov, float _near, float _far)
		: m_minX(0), m_minY(0), m_minZ(0), m_maxX(0), m_maxY(0), m_maxZ(0),
			m_farHeight(0), m_farWidth(0), m_nearHeight(0), m_nearWidth(0),
			m_nearPlane(_near), m_camera(NULL), m_cameraRotation(1)
	{
		m_vertices = new Math::Vec3[8];
		m_farWidth = SHADOW_DISTANCE * tanf(Math::ToRadians(fov));
		m_nearWidth = _near * tanf(Math::ToRadians(fov));
		auto aspectRatio = Application::Get().GetWindow().GetAspectRatio();
		m_farHeight = m_farWidth / aspectRatio;
		m_nearHeight = m_nearWidth / aspectRatio;
	}

	ShadowBox::~ShadowBox()
	{
		delete[] m_vertices;
	}

	bool ShadowBox::Update()
	{
		if (m_camera == NULL) { return false; }

		m_cameraRotation.SetIdentity();
		m_cameraRotation.Rotate(-m_camera->GetYaw(), Math::Vec3::Y_Axis);
		m_cameraRotation.Rotate(-m_camera->GetPitch(), Math::Vec3::X_Axis);
		Math::Vec3 forwardVector = m_cameraRotation * Math::Vec3::Forward;

		CalculateFrustumVertices(forwardVector,
								(forwardVector * m_nearPlane) + m_camera->GetTranslation(),  // Center Near
								(forwardVector * SHADOW_DISTANCE) + m_camera->GetTranslation()); // Center Far
		int i = 0;
		while (i < 8)
		{
			if (i == 0)
			{
				m_minX = m_vertices[i].x;
				m_maxX = m_vertices[i].x;
				m_minY = m_vertices[i].y;
				m_maxY = m_vertices[i].y;
				m_minZ = m_vertices[i].z;
				m_maxZ = m_vertices[i].z;
				i++;
				continue;
			}

			if (m_vertices[i].x > m_maxX)
			{
				m_maxX = m_vertices[i].x;
			}
			else if (m_vertices[i].x < m_minX)
			{
				m_minX = m_vertices[i].x;
			}

			if (m_vertices[i].y > m_maxY)
			{
				m_maxY = m_vertices[i].y;
			}
			else if (m_vertices[i].y < m_minY)
			{
				m_minY = m_vertices[i].y;
			}

			if (m_vertices[i].z > m_maxZ)
			{
				m_maxZ = m_vertices[i].z;
			}
			else if (m_vertices[i].z < m_minZ)
			{
				m_minZ = m_vertices[i].z;
			}
			i++;
		}
		m_maxZ += OFFSET;
		return true;
	}

	void ShadowBox::CalculateFrustumVertices(const Math::Vec3& forward, const Math::Vec3& centerNear, const Math::Vec3& centerFar)
	{
		Math::Vec3 upVector = m_cameraRotation * Math::Vec3::Y_Axis;
		Math::Vec3 rightVector = forward.Cross(upVector);
		Math::Vec3 downVector = upVector * -1;
		Math::Vec3 leftVector = rightVector * -1;

		Math::Vec3 farTop = centerFar + (upVector * m_farHeight);
		Math::Vec3 farBottom = centerFar + (downVector * m_farHeight);
		Math::Vec3 nearTop = centerNear + (upVector * m_nearHeight);
		Math::Vec3 nearBottom = centerNear + (downVector * m_nearHeight);

		m_vertices[0] = CALC_LIGHT_SPACE(farTop, rightVector, m_farWidth);
		m_vertices[1] = CALC_LIGHT_SPACE(farTop, leftVector, m_farWidth);
		m_vertices[2] = CALC_LIGHT_SPACE(farBottom, rightVector, m_farWidth);
		m_vertices[3] = CALC_LIGHT_SPACE(farBottom, leftVector, m_farWidth);
		m_vertices[4] = CALC_LIGHT_SPACE(nearTop, rightVector, m_nearWidth);
		m_vertices[5] = CALC_LIGHT_SPACE(nearTop, leftVector, m_nearWidth);
		m_vertices[6] = CALC_LIGHT_SPACE(nearBottom, rightVector, m_nearWidth);
		m_vertices[7] = CALC_LIGHT_SPACE(nearBottom, leftVector, m_nearWidth);
	}
}
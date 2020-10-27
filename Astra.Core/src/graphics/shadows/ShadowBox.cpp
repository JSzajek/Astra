#include "ShadowBox.h"

#include "../Window.h"

namespace Astra::Graphics
{
	ShadowBox::ShadowBox(const Math::Mat4& lightViewMatrix, Camera* camera, float fov, float near, float far)
		: m_lightViewMatrix(lightViewMatrix), m_camera(camera),
			m_minX(0), m_minY(0), m_minZ(0), m_maxX(0), m_maxY(0), m_maxZ(0),
			m_farHeight(0), m_farWidth(0), m_nearHeight(0), m_nearWidth(0),
			m_nearPlane(near)
	{
		m_farWidth = SHADOW_DISTANCE * tanf(Math::ToRadians(fov));
		m_nearWidth = near * tanf(Math::ToRadians(fov));
		m_farHeight = m_farWidth / Window::GetAspectRatio();
		m_farHeight = m_nearWidth / Window::GetAspectRatio();
	}

	const Math::Vec3& ShadowBox::GetCenter()
	{
		Math::Vec3 center((m_minX + m_maxX) / 2.0f,
						  (m_minY + m_maxY) / 2.0f,
						  (m_minZ + m_maxZ) / 2.0f);
		Math::Mat4 inverted = m_lightViewMatrix.Inverse();
		return inverted * center;
	}

	void ShadowBox::Update()
	{
		Math::Mat4 rotation = CalculateCameraRotationMatrix();
		Math::Vec3 forwardVector = rotation * Math::Forward;
		Math::Vec3 toFar = forwardVector * SHADOW_DISTANCE;
		Math::Vec3 toNear = forwardVector * m_nearPlane;
		Math::Vec3 centerNear = toNear + m_camera->GetPosition();
		Math::Vec3 centerFar = toNear + m_camera->GetPosition();

		Math::Vec4* points = CalculateFrustumVertices(rotation, forwardVector, centerNear, centerFar);
		int i = 0;
		while (i < 8)
		{
			if (i == 0)
			{
				m_minX = (*points).x;
				m_maxX = (*points).x;
				m_minY = (*points).y;
				m_maxY = (*points).y;
				m_minY = (*points).z;
				m_maxY = (*points).z;
				continue;
			}

			if ((*points).x > m_maxX)
			{
				m_maxX = (*points).x;
			}
			else if ((*points).x < m_minX)
			{
				m_minX = (*points).x;
			}

			if ((*points).y > m_maxY)
			{
				m_maxY = (*points).y;
			}
			else if ((*points).y < m_minY)
			{
				m_minY = (*points).y;
			}

			if ((*points).z > m_maxZ)
			{
				m_maxZ = (*points).z;
			}
			else if ((*points).z < m_minZ)
			{
				m_minZ = (*points).z;
			}

			points++;
			i++;
		}
		m_maxZ += OFFSET;
	}

	Math::Vec4* const ShadowBox::CalculateFrustumVertices(const Math::Mat4 rotation, const Math::Vec3& forward, const Math::Vec3& centerNear, const Math::Vec3& centerFar)
	{
		Math::Vec3 upVector = rotation * upVector;
		Math::Vec3 rightVector = forward.Cross(upVector);
		Math::Vec3 downVector = upVector * -1;
		Math::Vec3 leftVector = rightVector * -1;

		Math::Vec3 farTop = centerFar + (upVector * m_farHeight);
		Math::Vec3 farBottom = centerFar + (downVector * m_farHeight);
		Math::Vec3 nearTop = centerNear + (upVector * m_nearHeight);
		Math::Vec3 nearBottom = centerNear + (downVector * m_nearHeight);

		Math::Vec4* vertices = new Math::Vec4[8];
		vertices[0] = CalculateLightSpaceFrustumCorner(farTop, rightVector, m_farWidth);
		vertices[1] = CalculateLightSpaceFrustumCorner(farTop, leftVector, m_farWidth);
		vertices[2] = CalculateLightSpaceFrustumCorner(farBottom, rightVector, m_farWidth);
		vertices[3] = CalculateLightSpaceFrustumCorner(farBottom, leftVector, m_farWidth);
		vertices[4] = CalculateLightSpaceFrustumCorner(nearTop, rightVector, m_nearWidth);
		vertices[5] = CalculateLightSpaceFrustumCorner(nearTop, leftVector, m_nearWidth);
		vertices[6] = CalculateLightSpaceFrustumCorner(nearBottom, rightVector, m_nearWidth);
		vertices[7] = CalculateLightSpaceFrustumCorner(nearBottom, leftVector, m_nearWidth);
		return vertices;
	}

	const Math::Vec4& ShadowBox::CalculateLightSpaceFrustumCorner(const Math::Vec3& start, const Math::Vec3& direction, float width)
	{
		Math::Vec3 point(start + (direction * width));
		Math::Vec4 point4D(point, 1);
		return m_lightViewMatrix * point4D;
	}

	const Math::Mat4& ShadowBox::CalculateCameraRotationMatrix()
	{
		Math::Mat4 rotation(1);
		rotation = rotation.Rotate(-m_camera->GetYaw(), Math::YAxis);
		rotation = rotation.Rotate(m_camera->GetPitch(), Math::XAxis);
		return rotation;
	}
}
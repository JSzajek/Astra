#include "Camera.h"

namespace Astra::Graphics
{
	Camera::Camera()
		: Spatial(), m_pitch(75), m_distance(10), m_angleAroundFocal(0),
			m_focalPoint(Spatial())
	{
		UpdatePosition();
	}

	void Camera::UpdatePosition()
	{
		float horizontalDist = m_distance * cos(Math::ToRadians(m_pitch));
		float verticalDist = m_distance * sin(Math::ToRadians(m_pitch));
		
		float theta = m_focalPoint.rows[1].y + m_angleAroundFocal;
		float xOffset = horizontalDist * sin(Math::ToRadians(theta));
		float zOffset = horizontalDist * cos(Math::ToRadians(theta));
		rows[0].x = m_focalPoint.rows[0].x - xOffset;
		rows[0].y = m_focalPoint.rows[0].y + verticalDist;
		rows[0].z = m_focalPoint.rows[0].z - zOffset;
		m_yaw = 180 - (m_focalPoint.rows[1].y + m_angleAroundFocal);
	}

	void Camera::LookAt(const Math::Vec3& lookAtPoint)
	{
		m_focalPoint.rows[0].x = lookAtPoint.x;
		m_focalPoint.rows[0].y = lookAtPoint.y;
		m_focalPoint.rows[0].z = lookAtPoint.z;
	}
}
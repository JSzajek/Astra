#include "Camera.h"

#include "../../math/Mat4Utils.h"

namespace Astra::Graphics
{
	Camera::Camera(float pitch, float distance, float angleAround)
		: Spatial(), m_pitch(pitch), m_distance(distance), m_yaw(0),
				m_angleAroundFocal(angleAround), m_focalPoint(Spatial())
	{
		m_viewMatrix = new Math::Mat4(1);
		UpdatePosition();
	}

	Camera::Camera()
		: Spatial(), m_pitch(0), m_distance(0), m_yaw(0), 
			m_angleAroundFocal(0), m_focalPoint(Spatial())
	{
		m_viewMatrix = new Math::Mat4(1);
		UpdatePosition();
	}

	void Camera::UpdatePosition()
	{
		float horizontalDist = m_distance * cos(Math::ToRadians(m_pitch));
		float verticalDist = m_distance * sin(Math::ToRadians(m_pitch));
		
		float theta = m_focalPoint.GetRotation().y + m_angleAroundFocal;
		float xOffset = horizontalDist * sin(Math::ToRadians(theta));
		float zOffset = horizontalDist * cos(Math::ToRadians(theta));
		m_rows[0].x = m_focalPoint.GetTranslation().x - xOffset;
		m_rows[0].y = m_focalPoint.GetTranslation().y + verticalDist;
		m_rows[0].z = m_focalPoint.GetTranslation().z - zOffset;
		m_yaw = 180 - (m_focalPoint.GetRotation().y + m_angleAroundFocal);
		Camera::UpdateMatrices();
	}

	void Camera::InvertPitch(float yDelta)
	{
		Spatial::operator[](0).y += yDelta; // change y
		m_pitch = -m_pitch;
		Camera::UpdateMatrices();
	}

	void Camera::LookAt(const Math::Vec3& lookAtPoint)
	{
		m_focalPoint.SetTranslation(lookAtPoint);
		UpdatePosition();
	}

	void Camera::operator()(unsigned int _type, unsigned int _op, unsigned int _index, float _val)
	{
		switch (_type)
		{
		case PITCH:
			UpdateValue(m_pitch, _op, _val);
			break;
		case SWIVEL:
			UpdateValue(m_angleAroundFocal, _op, _val);
			break;
		case DISTANCE:
			UpdateValue(m_distance, _op, _val);
			break;
		default:
			Spatial::operator()(_type, _op, _index, _val);
			break;
		}
		UpdatePosition();
	}

	void Camera::UpdateMatrices()
	{
		*m_viewMatrix = Math::Mat4Utils::ViewMatrix(this);
	}

	void Camera::UpdateValue(float& _updating, unsigned int _op, float _val)
	{
		switch (_op)
		{
		case SUM_EQ:
			_updating += _val;
			break;
		case SUB_EQ:
			_updating -= _val;
			break;
		case EQ:
			_updating = _val;
			break;
		}
	}
}
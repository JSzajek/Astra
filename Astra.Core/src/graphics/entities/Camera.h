#pragma once

#include "Spatial.h"

namespace Astra::Graphics
{
#define		MAX_DISTANCE	 80
#define		MIN_DISTANCE	 80
#define		MAX_PITCH		 90
#define		MIN_PITCH		-90

	class Camera : public Spatial
	{
	private:
		float m_pitch, m_distance, m_angleAroundFocal, m_yaw;
		Spatial m_focalPoint;
	public:
		Camera();
		void UpdatePosition();

		inline void SetPitch(float value) { m_pitch = value; }
		inline void SetSwivel(float value) { m_angleAroundFocal = value; }
		inline void SetDistance(float value) { m_distance = value; }

		inline float GetPitch() const { return m_pitch; }
		inline float GetSwivel() const { return m_angleAroundFocal; }
		inline float GetDistance() const { return m_distance; }
		inline float GetYaw() const { return m_yaw; }
		inline const Math::Vec3& GetPosition() const { return rows[0]; }
	};
}
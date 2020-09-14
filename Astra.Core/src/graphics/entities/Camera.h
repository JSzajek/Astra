#pragma once

#include "Spatial.h"

namespace Astra::Graphics
{
	class Camera : public Spatial
	{
	private:
		float m_pitch, m_distance, m_angleAroundFocal, m_yaw;
		Spatial m_focalPoint;
	public:
		Camera(float pitch, float distance, float angleAround);
		Camera();
		void UpdatePosition();

		inline void InvertPitch() { m_pitch = -m_pitch; }

		inline const float& GetPitch() const { return m_pitch; }
		inline const float& GetYaw() const { return m_yaw; }
		inline const Math::Vec3& GetPosition() const { return rows[0]; }
		inline const Math::Vec3* GetInvertedPosition() const { return &(rows[0] * -1); }

		inline float& Pitch() { return m_pitch; }
		inline float& Swivel() { return m_angleAroundFocal; }
		inline float& Distance() { return m_distance; }

		inline Math::Vec3& Position() { return rows[0]; }
		void LookAt(const Math::Vec3& lookAtPoint);

	};
}
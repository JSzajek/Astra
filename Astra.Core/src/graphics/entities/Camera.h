#pragma once

#include "Spatial.h"

namespace Astra::Graphics
{
	#define PITCH 3
	#define SWIVEL 4
	#define DISTANCE 5

	class Camera : public Spatial
	{
	private:
		float m_pitch, m_distance, m_angleAroundFocal, m_yaw;
		Spatial m_focalPoint;
		Math::Mat4* m_viewMatrix;
	public:
		Camera(float pitch, float distance, float angleAround);
		Camera();
		void UpdatePosition();

		inline void InvertPitch() 
		{ 
			m_pitch = -m_pitch;
			Camera::UpdateMatrices();
		}

		inline const Math::Mat4* const GetViewMatrix() const { return m_viewMatrix; }

		inline const float GetPitch() const { return m_pitch; }
		inline const float GetYaw() const { return m_yaw; }
		inline const float GetDistance() const { return m_distance; }
		inline const Math::Vec3& GetPosition() const { return rows[0]; }
		inline const Math::Vec3* GetInvertedPosition() const { return &(rows[0] * -1); }

		inline float& Pitch() { return m_pitch; }
		inline float& Yaw() { return m_yaw; }
		inline float& Swivel() { return m_angleAroundFocal; }
		inline float& Distance() { return m_distance; }

		inline Math::Vec3& Position() { return rows[0]; }
		void LookAt(const Math::Vec3& lookAtPoint);

		void operator()(unsigned int _type, unsigned int _op, unsigned int _index, float _val) override;
	protected:
		void UpdateMatrices() override;
	private:
		void UpdateValue(float& _updating, unsigned int _op, float _val);
	};
}
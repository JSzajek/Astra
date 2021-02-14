#pragma once

#include "../entities/Camera.h"
#include "../../math/Maths.h"

namespace Astra::Graphics
{
	#define OFFSET					15.0f
	#define TRANSITION_DISTANCE		10.0f
	#define SHADOW_DISTANCE			150.0f

	#define CALC_LIGHT_SPACE(start, dir, width) start + (dir * width);

	class ShadowBox
	{
	private:
		float m_minX, m_maxX;
		float m_minY, m_maxY;
		float m_minZ, m_maxZ;
		Camera* m_camera;
		Math::Vec3* m_vertices;
		Math::Mat4 m_cameraRotation;
		float m_farHeight, m_farWidth, m_nearHeight, m_nearWidth;
		float m_nearPlane;
	public:
		ShadowBox(float fov, float near, float far);
		~ShadowBox();

		inline void SetCamera(Camera* camera) { m_camera = camera; }

		inline const float GetWidth() const { return m_maxX - m_minX; }
		inline const float GetHeight() const { return m_maxY - m_minY; }
		inline const float GetLength() const { return m_maxZ - m_minZ; }

		bool Update();
	private:
		void CalculateFrustumVertices(const Math::Vec3& forward, const Math::Vec3& centerNear, const Math::Vec3& centerFar);
	};
}
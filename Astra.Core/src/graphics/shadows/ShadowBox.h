#pragma once

#include "../entities/Camera.h"
#include "../../math/Maths.h"

namespace Astra::Graphics
{
	#define OFFSET					15.0f
	#define TRANSITION_DISTANCE		10.0f
	#define SHADOW_DISTANCE			150.0f

	class ShadowBox
	{
	private:
		float m_minX, m_maxX;
		float m_minY, m_maxY;
		float m_minZ, m_maxZ;
		Math::Mat4 m_lightViewMatrix;
		Camera* m_camera;
		float m_farHeight, m_farWidth, m_nearHeight, m_nearWidth;
		float m_nearPlane;
	public:
		ShadowBox(const Math::Mat4& lightViewMatrix, Camera* camera, float fov, float near, float far);
		
		inline void SetCamera(Camera* camera) { m_camera = camera; }

		inline const float GetWidth() const { return m_maxX - m_minX; }
		inline const float GetHeight() const { return m_maxY - m_minY; }
		inline const float GetLength() const { return m_maxZ - m_minZ; }
		const Math::Vec3& GetCenter();

		bool Update();
	private:
		Math::Vec3* const CalculateFrustumVertices(const Math::Mat4& rotation, const Math::Vec3& forward, const Math::Vec3& centerNear, const Math::Vec3& centerFar);
		const Math::Vec3& CalculateLightSpaceFrustumCorner(const Math::Vec3& start, const Math::Vec3& direction, float width);
		const Math::Mat4& CalculateCameraRotationMatrix();
	};
}
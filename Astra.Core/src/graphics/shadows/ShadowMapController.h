#pragma once

#include "ShadowBox.h"
#include "../entities/Light.h"
#include "../buffers/ShadowFrameBuffer.h"
#include "../renderers/ShadowMapRenderer.h"

namespace Astra::Graphics
{
	#define SHADOW_MAP_SIZE		2048
	#define PCF_COUNT			2
	
	class ShadowMapController
	{
	private:
		ShadowFrameBuffer* m_buffer;
		ShadowShader* m_shader;
		ShadowMapRenderer* m_renderer;
		ShadowBox* m_box;

		Light* m_light;

		Math::Mat4 m_projectionMatrix;
		Math::Mat4 m_lightViewMatrix;
		Math::Mat4 m_projectionViewMatrix;
		Math::Mat4 m_offset;
	public:
		ShadowMapController(float fov, float near, float far);
		~ShadowMapController();
		
		void Render();
		void SetDirectionalLight(Light* light);
		inline void Clear() { m_renderer->Clear(); }

		inline void SetCamera(Camera* camera) { m_box->SetCamera(camera); }
		inline void AddEntity(const Entity* entity) { m_renderer->AddEntity(entity); }

		inline Math::Mat4 GetToShadowMapSpaceMatrix() const { return m_offset * m_projectionViewMatrix; }
		inline const unsigned int GetShadowMap() const { return m_buffer->GetShadowMap(); }
		inline const Math::Mat4& GetLightSpaceTransform() const { return m_lightViewMatrix; }
	private:
		void Prepare();
		void UpdateLightViewMatrix(Math::Vec3 direction);
		void UpdateOrthoProjectionMatrix(float width, float height, float length);
		const Math::Mat4& CreateOffset();
	};
}
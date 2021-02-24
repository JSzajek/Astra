#pragma once

#include "ShadowBox.h"
#include "Astra/graphics/entities/Model.h"
#include "Astra/graphics/entities/DirectionalLight.h"
#include "Astra/graphics/buffers/ShadowFrameBuffer.h"
#include "Astra/graphics/renderers/ShadowMapRenderer.h"

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

		const DirectionalLight* m_light;

		Math::Mat4 m_projectionMatrix;
		Math::Mat4 m_lightViewMatrix;
		Math::Mat4 m_projectionViewMatrix;
		Math::Mat4 m_offset;
	public:
		ShadowMapController(float fov, float _near, float _far);
		~ShadowMapController();
		
		void Render(const std::unordered_map<unsigned int, std::vector<const Model*>>& models);
		void SetDirectionalLight(const DirectionalLight* light);

		inline void SetCamera(Camera* camera) { m_box->SetCamera(camera); }

		inline Math::Mat4 GetToShadowMapSpaceMatrix() const { return m_offset * m_projectionViewMatrix; }
		inline const unsigned int GetShadowMap() const { return m_buffer->GetShadowMap(); }
		inline const Math::Mat4& GetLightSpaceTransform() const { return m_lightViewMatrix; }
	private:
		void Prepare();
		void UpdateLightViewMatrix(Math::Vec3 direction);
		void UpdateOrthoProjectionMatrix(float width, float height, float length);
		const Math::Mat4 CreateOffset();
	};
}
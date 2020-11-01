#pragma once

#include "Shader.h"

namespace Astra::Graphics
{	
	#define NORMAL_MATRIX				"normalMatrix"

	#define DIFFUSE_MAP					"material.diffuseMap"
	#define SPECULAR_MAP				"material.specularMap"
	#define MATERIAL_REFLECTIVITY		"material.reflectivity"

	#define DIR_LIGHT_DIRECTION			"directionalLight.direction"
	#define DIR_LIGHT_AMBIENT			"directionalLight.ambient"
	#define DIR_LIGHT_DIFFUSE			"directionalLight.diffuse"
	#define DIR_LIGHT_SPECULAR			"directionalLight.specular"

	class EntityShader : public Shader
	{
	public:
		static constexpr const char* LightPositionTag = "lightPosition";
		static constexpr const char* LightColorTag = "lightPosition";
	public:
		EntityShader(int numOfLights, const char* filepath =
			"../Astra.Core/src/resources/shaders/Entity.shader")
			: Shader(filepath, ShaderType::Lighting, &std::make_tuple("NR_POINT_LIGHTS %i", numOfLights))
		{
		}

		static const char* GetPointLightPositionTag(int index)
		{
			sprintf(GetBuffer(), "pointLights[%i].position", index);
			return GetBuffer();
		}

		static const char* GetPointLightAmbientTag(int index)
		{
			sprintf(GetBuffer(), "pointLights[%i].ambient", index);
			return GetBuffer();
		}

		static const char* GetPointLightDiffuseTag(int index)
		{
			sprintf(GetBuffer(), "pointLights[%i].diffuse", index);
			return GetBuffer();
		}

		static const char* GetPointLightSpecularTag(int index)
		{
			sprintf(GetBuffer(), "pointLights[%i].specular", index);
			return GetBuffer();
		}

		static const char* GetPointLightAttenuationTag(int index)
		{
			sprintf(GetBuffer(), "pointLights[%i].attenuation", index);
			return GetBuffer();
		}
	private:
		static char* GetBuffer()
		{
			static char m_buffer[64];
			return m_buffer;
		}
	};
}
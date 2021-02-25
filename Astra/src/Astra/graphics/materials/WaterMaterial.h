#pragma once

#include "Astra/graphics/buffers/Texture.h"

namespace Astra::Graphics
{
	class WaterMaterial
	{
	public:
		static const char* DefaultDiffuseMap;
		static const char* DefaultDuDvMap;
		static const char* DefaultNormalMap;
	private:
		static const constexpr float DefaultWaveStrength = 0.02f;
		static const constexpr float DefaultWaveSpeed = 0.03f;
		static const constexpr float DefaultShineDampener = 20.0f;
		static const constexpr float DefaultReflectivity = 0.6f;

	private:
		float m_currentWaveSpeed;
		Asset<Texture> m_specular;
	public:
		float waveStrength, waveSpeed;
		float shineDampener, reflectivity;
		Asset<Texture> diffuseTexture;
		Asset<Texture> dudvTexture;
		Asset<Texture> normalTexture;
	public:
		WaterMaterial();
		WaterMaterial(const char* const diffuseFilePath, const char* const dudvMapFilePath, const char* const normalMapFilePath);
		WaterMaterial(const WaterMaterial& other);
		
		inline unsigned int GetSpecularId() const { return m_specular->id; }

		float Increase(float delta);
	};
}
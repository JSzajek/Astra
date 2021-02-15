#pragma once

#include "../buffers/Texture.h"
#include "../../math/Vec4.h"

namespace Astra::Graphics
{
	class WaterMaterial
	{
	private:
		static const constexpr float DefaultWaveStrength = 0.02f;
		static const constexpr float DefaultWaveSpeed = 0.03f;
		static const constexpr float DefaultShineDampener = 20.0f;
		static const constexpr float DefaultReflectivity = 0.6f;

		static const char* DefaultDiffuseMap;
		static const char* DefaultDuDvMap;
		static const char* DefaultNormalMap;
	private:
		float m_currentWaveSpeed;
		const Texture* m_specular;
	public:
		float waveStrength, waveSpeed;
		float shineDampener, reflectivity;
		const Texture* diffuseTexture;
		const Texture* dudvTexture;
		const Texture* normalTexture;
	public:
		WaterMaterial();
		WaterMaterial(const char* const dudvMapFilePath, const char* const normalMapFilePath);
		~WaterMaterial();
		
		inline unsigned int GetSpecularId() const { return m_specular->id; }

		float Increase(float delta);
	};
}
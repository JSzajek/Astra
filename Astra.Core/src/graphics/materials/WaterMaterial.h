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

		static constexpr const char* DefaultDiffuseMap = "../Astra.Core/src/resources/textures/DefaultWaterBase.jpg";
		static constexpr const char* DefaultDuDvMap = "../Astra.Core/src/resources/textures/DefaultDuDvMap.png";
		static constexpr const char* DefaultNormalMap = "../Astra.Core/src/resources/textures/DefaultNormalMap.png";
	private:
		float m_currentWaveSpeed;
		Texture m_specular;
	public:
		float waveStrength, waveSpeed;
		float shineDampener, reflectivity;
		Texture diffuseTexture;
		Texture dudvTexture;
		Texture normalTexture;
	public:
		WaterMaterial();
		WaterMaterial(const char* const dudvMapFilePath, const char* const normalMapFilePath);
		
		inline unsigned int GetSpecularId() const { return m_specular.id; }

		float Increase();
	};
}
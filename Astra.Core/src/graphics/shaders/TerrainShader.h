#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class TerrainShader : public Shader
	{
	#define MAX_LIGHTS 4

	public:
		static constexpr const char* LightPositionTag = "lightPosition";
		static constexpr const char* LightColorTag = "lightColor";
		static constexpr const char* ShineDampenerTag = "shineDampener";
		static constexpr const char* ReflectivityTag = "reflectivity";

		static constexpr const char* SkyColorTag = "fogColor";

		static constexpr const char* BackgroundTextureTag = "backgroundTexture";
		static constexpr const char* RTextureTag = "rTexture";
		static constexpr const char* GTextureTag = "gTexture";
		static constexpr const char* BTextureTag = "bTexture";
		static constexpr const char* BlendMapTag = "blendMap";
		static constexpr const char* ShadowMapTag = "shadowMap";

		static constexpr const char* ClipPaneTag = "clipPlane";

		static constexpr const char* ToShadowSpaceMatrixTag = "toShadowMapSpace";
		static constexpr const char* ShadowDistanceTag = "shadowDistance";
		static constexpr const char* TransitionDistanceTag = "transitionDistance";
		static constexpr const char* MapSizeTag = "mapSize";
		static constexpr const char* PcfCountTag = "pcfCount";
	public:
		TerrainShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/Terrain.shader")
			: Shader(filepath, ShaderType::Terrains)
		{
		}

		static const char* GetLightPositionTag(int index)
		{
			static std::string base("lightPosition[x]");
			return base.replace(base.end() - 2, base.end() - 1, std::to_string(index)).c_str();
		}

		static const char* GetLightColorTag(int index)
		{
			static std::string base("lightColor[x]");
			return base.replace(base.end() - 2, base.end() - 1, std::to_string(index)).c_str();
		}

		static const char* GetAttenuationTag(int index)
		{
			static std::string base("attenuation[x]");
			return base.replace(base.end() - 2, base.end() - 1, std::to_string(index)).c_str();
		}
	};
}
#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <tuple>

#include "../../math/Maths.h"
#include "../../utils/FileUtils.h"

namespace Astra::Graphics
{
	enum ShaderType
	{
		Basic,
		Lighting,
		Gui,
		Terrains,
		Skybox,
		Water,
		NormalMapped,
		Particles,
		Shadows
	};

	struct ShaderProgramSource
	{
		std::string VertextSource;
		std::string FragmentSource;
	};

	#define PROJECTION_MATRIX_TAG		"projectionMatrix"
	#define VIEW_MATRIX_TAG				"viewMatrix"
	#define TRANSFORM_MATRIX_TAG		"transformMatrix"
	#define NORMAL_MATRIX_TAG			"normalMatrix"

	#define INVERSE_VIEW_VECTOR_TAG		"inverseViewVector"
	#define CLIP_PLANE					"clipPlane"
	#define FOG_COLOR					"fogColor"

	#define DIFFUSE_MAP					"material.diffuseMap"
	#define SPECULAR_MAP				"material.specularMap"
	#define MATERIAL_REFLECTIVITY		"material.reflectivity"

	#define DIR_LIGHT_DIRECTION			"directionalLight.direction"
	#define DIR_LIGHT_AMBIENT			"directionalLight.ambient"
	#define DIR_LIGHT_DIFFUSE			"directionalLight.diffuse"
	#define DIR_LIGHT_SPECULAR			"directionalLight.specular"

	// Shadow based tags
	#define SHADOW_MAP_TAG				"material.shadowMap"
	#define TO_SHADOW_SPACE_MATRIX_TAG	"toShadowMapSpace"
	#define SHADOW_DISTANCE_TAG			"shadowDistance"
	#define TRANSITION_DISTANCE_TAG		"transitionDistance"
	#define SHADOW_MAP_SIZE_TAG			"mapSize"
	#define PCF_COUNT_TAG				"pcfCount"

	class Shader
	{
	private:
		GLuint m_id;
		const char* m_filepath;
		std::unordered_map<std::string, GLint> indexDirectory;
		std::unordered_map<std::string, GLint> failedDirectory;
		ShaderType m_type;
		std::tuple<const char*, int>* m_replace;
	public:
		Shader(const char* filepath, ShaderType type = ShaderType::Basic, std::tuple<const char*, int>* replace = NULL);
		~Shader();

		void SetUniform1i(const GLchar* name, const int& value);
		void SetUniform1f(const GLchar* name, const float& value);
		void SetUniform2f(const GLchar* name, const Math::Vec2& value);
		void SetUniform2f(const GLchar* name, const float& v1, const float& v2);
		void SetUniform3f(const GLchar* name, const Math::Vec3& value);
		void SetUniform4f(const GLchar* name, const Math::Vec4& value);
		void SetUniform4f(const GLchar* name, const Math::Vec3& vector, float w);
		virtual void SetUniformMat4(const GLchar* name, const Math::Mat4& matrix);
		void SetUniformMat4(const GLchar* name, const Math::Mat4* const matrix);

		void Start() const;
		void Stop() const;

		inline const ShaderType& GetType() const { return m_type; }
	protected:
		inline void BindAttribute(GLuint index, const char* const name) const { glBindAttribLocation(m_id, index, name); }
		GLint GetUniformLocation(const GLchar* name);
	private:
		void BindAttributes() const;
		ShaderProgramSource ParseShader(const std::string& filepath);
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	public:
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
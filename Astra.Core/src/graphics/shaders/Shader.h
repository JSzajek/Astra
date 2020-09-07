#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

#include "../../math/Maths.h"
#include "../../utils/FileUtils.h"

namespace Astra::Graphics
{
	enum ShaderType
	{
		Basic,
		Lighting,
		Gui,
		Terrains
	};

	struct ShaderProgramSource
	{
		std::string VertextSource;
		std::string FragmentSource;
	};

	class Shader
	{
	public:
		static constexpr const char* ProjectionMatrixTag = "projectionMatrix";
		static constexpr const char* ViewMatrixTag = "viewMatrix";
		static constexpr const char* TransformMatrixTag = "transformMatrix";
	private:
		GLuint m_id;
		const char* m_filepath;
		std::unordered_map<std::string, GLint> indexDirectory;
		std::unordered_map<std::string, GLint> failedDirectory;
		ShaderType m_type;
	public:
		Shader(const char* filepath, ShaderType type = ShaderType::Basic);
		~Shader();

		void SetUniform1i(const GLchar* name, const int& value);
		void SetUniform1f(const GLchar* name, const float& value);
		void SetUniform2f(const GLchar* name, const Math::Vec2& value);
		void SetUniform2f(const GLchar* name, const float& v1, const float& v2);
		void SetUniform3f(const GLchar* name, const Math::Vec3& value);
		void SetUniform4f(const GLchar* name, const Math::Vec4& value);
		void SetUniformMat4(const GLchar* name, const Math::Mat4& matrix);

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
	};
}
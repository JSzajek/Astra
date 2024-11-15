#include "astra_pch.h"

#include "Shader.h"
#include "Astra/Log.h"

namespace Astra::Graphics
{
	Shader::Shader(const char* filpath, std::tuple<const char*, int>* replace)
		: m_filepath(filpath), m_replace(replace)
	{
		ShaderProgramSource source = ParseShader(m_filepath);
		m_id = CreateShader(source.VertextSource, source.FragmentSource);

		Start();
		BindAttributes();
		Stop();
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_id);
	}

	void Shader::SetUniform1i(const GLchar* name, const int& value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void Shader::SetUniform1iv(const GLchar* name, int count, const int* value)
	{
		glUniform1iv(GetUniformLocation(name), count, value);
	}

	void Shader::SetUniform1f(const GLchar* name, const float& value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void Shader::SetUniform2f(const GLchar* name, const Math::Vec2& vector)
	{
		glUniform2f(GetUniformLocation(name), vector.x, vector.y);
	}

	void Shader::SetUniform2f(const GLchar* name, const float& v1, const float& v2)
	{
		glUniform2f(GetUniformLocation(name), v1, v2);
	}

	void Shader::SetUniform3f(const GLchar* name, const Math::Vec3& vector)
	{
		glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z);
	}

	void Shader::SetUniform4f(const GLchar* name, const Math::Vec4& vector)
	{
		glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
	}

	void Shader::SetUniform4f(const GLchar* name, const Math::Vec3& vector, float w)
	{
		glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, w);
	}

	void Shader::SetUniformMat4(const GLchar* name, const Math::Mat4* const matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix->data);
	}

	void Shader::SetUniformMat4(const GLchar* name, const Math::Mat4& matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix.data);
	}

	void Shader::Start() const
	{
		glUseProgram(m_id);
	}

	void Shader::Stop() const
	{
		glUseProgram(0);
	}

	void Shader::BindAttributes() const
	{
		BindAttribute(0, "position");
		BindAttribute(1, "textureCoords");
		BindAttribute(2, "normal");
		BindAttribute(3, "tangent");
	}

	ShaderProgramSource Shader::ParseShader(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		ASTRA_CORE_ASSERT(stream, "Shader: Couldn't Open Shader File.");

		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;
		while (getline(stream, line))
		{
			if (m_replace != NULL && line.find(std::get<0>(*m_replace)) != std::string::npos)
			{
				auto* replacementString = std::get<0>(*m_replace);
				char buffer[64];
				sprintf(buffer, line.c_str(), std::get<1>(*m_replace));
				line = std::string(buffer);
			}

			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{
					type = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos)
				{
					type = ShaderType::FRAGMENT;
				}
			}
			else
			{
				ss[static_cast<int>(type)] << line << std::endl;
			}
		}
		return { ss[0].str(), ss[1].str() };
	}

	unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
	{
		GLuint id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		GLint result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

			std::vector<char> error(length);
			glGetShaderInfoLog(id, length, &length, &error[0]);
			ASTRA_CORE_ERROR("Shader: Failed to Compile {0} Shader exiting with Error: {1}.", std::string(type == GL_VERTEX_SHADER ? "vertex" : "fragment"), &error[0]);
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	GLuint Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		GLuint program = glCreateProgram();
		GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		// Clean up shaders
		glDeleteShader(vs);
		glDeleteShader(fs);

		Stop();

		return program;
	}

	GLint Shader::GetUniformLocation(const GLchar* name)
	{
		auto found = indexDirectory.find(name);
		if (found != indexDirectory.end())
		{
			return found->second;
		}
		else
		{
			if (failedDirectory.find(name) != failedDirectory.end())
			{
				return -1;
			}

			GLint index = glGetUniformLocation(m_id, name);
			if (index == -1)
			{
				ASTRA_CORE_WARN("Shader: Failed to Find Uniform {0}.", name);
				failedDirectory[name] = index;
			}
			else
			{
				indexDirectory[name] = index;
			}
			return index;
		}
	}
}
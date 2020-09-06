#include "Shader.h"
#include "../../logger/Logger.h"

namespace Astra::Graphics
{
	Shader::Shader(const char* filpath, ShaderType type)
		: m_filepath(filpath), m_type(type)
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

	void Shader::SetUniform1f(const GLchar* name, const float& value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void Shader::SetUniform2f(const GLchar* name, const Math::Vec2& vector)
	{
		glUniform2f(GetUniformLocation(name), vector.x, vector.y);
	}

	void Shader::SetUniform3f(const GLchar* name, const Math::Vec3& vector)
	{
		glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z);
	}

	void Shader::SetUniform4f(const GLchar* name, const Math::Vec4& vector)
	{
		glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
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
	}

	ShaderProgramSource Shader::ParseShader(const std::string& filepath)
	{
		std::ifstream stream(filepath);

		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;
		while (getline(stream, line))
		{
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
				ss[int(type)] << line << std::endl;
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
			Logger::LogError(std::string("Failed to Compile ") + std::string(type == GL_VERTEX_SHADER ? "vertex" : "fragment") + " shader!");
			Logger::LogError(&error[0]);
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
				Logger::LogWarning(std::string("Failed to find Uniform ") + name);
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
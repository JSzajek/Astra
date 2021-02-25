#pragma once

#include "Astra/graphics/shaders/Shader.h"
#include "Astra/graphics/entities/Light.h"

namespace Astra::Graphics
{
#if ASTRA_DEBUG
	static GLenum glCheckError_(const char* file, int line)
	{
		GLenum errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
				case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
				case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
				case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
				case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
				case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
				case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
				case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			}
			std::cout << error << " | " << file << " (" << line << ")" << std::endl;
		}
		return errorCode;
	}
	#define glCheckError() glCheckError_(__FILE__, __LINE__) 
#endif
	class Renderer
	{
	public:
		static const Math::Vec4 DefaultClipPlane;
	protected:
		Shader* m_shader;
	public:
		virtual void UpdateProjectionMatrix(const Math::Mat4* projectionMatrix);
		void BindFrameBuffer(unsigned int bufferId, unsigned int width, unsigned int height);
		void UnbindFrameBuffer();
	protected:
		Renderer();
		~Renderer();

		virtual void SetShader(Shader* shader);
		virtual void UnbindVertexArray();
	public:
		void UpdateLight(unsigned int index, const Light* light);
	};
}
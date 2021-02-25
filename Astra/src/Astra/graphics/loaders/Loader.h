#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "Astra/graphics/buffers/FrameBuffer.h"
#include "Astra/graphics/buffers/WaterFrameBuffer.h"
#include "Astra/graphics/buffers/ShadowFrameBuffer.h"

namespace Astra::Graphics
{
	// Copied values from glew to prevent vendor package in header files
	#define ASTRA_GL_NONE				0
	#define ASTRA_GL_REPEAT				0x2901
	#define ASTRA_GL_LINEAR				0x2601
	#define ASTRA_GL_DEPTH24_STENCIL8	0x88F0
	
	class Loader
	{
	public:
		Loader(const Loader&) = delete;
		void operator=(const Loader&) = delete;

		static Loader& Get()
		{
			static Loader instance;
			return instance;
		}
		
		static WaterFrameBuffer* LoadWaterFrameBuffer(unsigned int reflectionWidth, unsigned int reflectionHeight,
													  unsigned int refractionWidth, unsigned int refractionHeight)
		{
			return Get().LoadWaterFrameBufferImpl(reflectionWidth, reflectionHeight, refractionWidth, refractionHeight);
		}

		static ShadowFrameBuffer* LoadShadowFrameBuffer(unsigned int width, unsigned int height)
		{
			return Get().LoadShadowFrameBufferImpl(width, height);
		}

		static FrameBuffer* LoadFrameBuffer(const FrameBufferCreationSpec& specs)
		{
			return Get().LoadFrameBufferImpl(specs);
		}

		static FrameBuffer* LoadMultiTargetFrameBuffer(const MultiTargetFrameBufferCreationSpec& specs)
		{
			return Get().LoadMultiTargetFrameBufferImpl(specs);
		}

		static void UpdateFrameBuffer(FrameBuffer* buffer, unsigned int width, unsigned int height, bool floating, unsigned int multisampled)
		{
			return Get().UpdateFrameBufferImpl(buffer, width, height, floating, multisampled);
		}

	private:
		Loader() = default;

		// Specialized Frame Buffers
		WaterFrameBuffer* LoadWaterFrameBufferImpl(unsigned int reflectionWidth, unsigned int reflectionHeight,
												   unsigned int refractionWidth, unsigned int refractionHeight);
		ShadowFrameBuffer* LoadShadowFrameBufferImpl(unsigned int width, unsigned int height);
		
		// Load Basic Frame Buffers
		FrameBuffer* LoadFrameBufferImpl(const FrameBufferCreationSpec& specs);
		FrameBuffer* LoadMultiTargetFrameBufferImpl(const MultiTargetFrameBufferCreationSpec& specs);

		void UpdateFrameBufferImpl(FrameBuffer* buffer, unsigned int width, unsigned int height, bool floating, unsigned int multisampled);

		FrameBuffer* CreateFrameBuffer(DepthBufferType type, bool multisampled = false, int drawAttachment = ASTRA_GL_NONE, int readAttachment = ASTRA_GL_NONE);
		void CreateTextureAttachment(unsigned int& id, unsigned int width, unsigned int height, bool floating, unsigned int wrapping = ASTRA_GL_REPEAT, size_t offset = 0);
		void CreateDepthTextureAttachment(unsigned int& id, unsigned int width, unsigned int height, int component = ASTRA_GL_DEPTH24_STENCIL8, int filter = ASTRA_GL_LINEAR, int wrap = ASTRA_GL_REPEAT);
		void CreateDepthBufferAttachment(unsigned int& id, unsigned int width, unsigned int height, unsigned int multisampled = 0, bool floating = false);
		void CreateColorBufferAttachment(unsigned int& id, unsigned int width, unsigned int height, unsigned int multisampled = 0, bool floating = false);
		
		void UnbindFrameBuffer();
	};
}
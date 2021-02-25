#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "Astra/graphics/buffers/FrameBuffer.h"
#include "Astra/graphics/buffers/WaterFrameBuffer.h"
#include "Astra/graphics/buffers/ShadowFrameBuffer.h"

namespace Astra::Graphics
{
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

		static FrameBuffer* LoadFrameBuffer(unsigned int width, unsigned int height, unsigned int multisampled = 0, DepthBufferType depthType = DepthBufferType::None, bool floating = false, unsigned int wrapping = GL_REPEAT)
		{
			return Get().LoadFrameBufferImpl(width, height, multisampled, depthType, floating, wrapping);
		}

		static FrameBuffer* LoadMultiTargetFrameBuffer(unsigned int width, unsigned int height, size_t colorAttachments, size_t depthAttachments = 1, bool floating = false)
		{
			return Get().LoadMultiTargetFrameBufferImpl(width, height, colorAttachments, depthAttachments, floating);
		}

		static void UpdateFrameBuffer(FrameBuffer* buffer, unsigned int width, unsigned int height, bool floating, unsigned int multisampled)
		{
			return Get().UpdateFrameBufferImpl(buffer, width, height, floating, multisampled);
		}

	private:
		Loader() = default;

		WaterFrameBuffer* LoadWaterFrameBufferImpl(unsigned int reflectionWidth, unsigned int reflectionHeight,
												   unsigned int refractionWidth, unsigned int refractionHeight);
		ShadowFrameBuffer* LoadShadowFrameBufferImpl(unsigned int width, unsigned int height);
		FrameBuffer* LoadFrameBufferImpl(unsigned int width, unsigned int height, unsigned int multisampled, DepthBufferType depthType, bool floating, unsigned int component);
		FrameBuffer* LoadMultiTargetFrameBufferImpl(unsigned int width, unsigned int height, size_t colorAttachments, size_t depthAttachments, bool floating);

		void UpdateFrameBufferImpl(FrameBuffer* buffer, unsigned int width, unsigned int height, bool floating, unsigned int multisampled);

		FrameBuffer* CreateFrameBuffer(DepthBufferType type, bool multisampled = false, int drawAttachment = GL_NONE, int readAttachment = GL_NONE);
		void CreateTextureAttachment(unsigned int& id, unsigned int width, unsigned int height, bool floating, unsigned int wrapping = GL_REPEAT, size_t offset = 0);
		unsigned int CreateDepthTextureAttachment(unsigned int& id, unsigned int width, unsigned int height, int component = GL_DEPTH24_STENCIL8, int filter = GL_LINEAR, int wrap = GL_REPEAT);
		void CreateDepthBufferAttachment(unsigned int& id, unsigned int width, unsigned int height, unsigned int multisampled = 0, bool floating = false);
		void CreateColorBufferAttachment(unsigned int& id, unsigned int width, unsigned int height, unsigned int multisampled = 0, bool floating = false);
		
		void UnbindFrameBuffer();
	};
}
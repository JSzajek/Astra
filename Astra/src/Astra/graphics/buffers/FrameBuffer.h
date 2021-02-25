#pragma once

#include <vector>

namespace Astra::Graphics
{
	#define ASTRA_GL_REPEAT		0x2901

	enum class DepthBufferType
	{
		Render,
		Texture,
		None
	};

	struct FrameBuffer
	{
	private:
		std::vector<unsigned int> m_data;
		size_t m_colorAttachmentOffset;
		size_t m_depthAttachements;
		DepthBufferType m_type;
		bool m_multisampled;
	public:
		FrameBuffer(DepthBufferType type, bool multisampled = false, size_t numOfColorAttachments = 1, size_t numOfDepthAttachments = 1);
		~FrameBuffer();

		const unsigned int& GetId() const { return m_data[0]; }
		const unsigned int& GetColorAttachment(size_t index = 0) const { return m_data[1 + index]; }
		const unsigned int& GetDepthAttachment(size_t index = 0) const { return m_data[1 + m_colorAttachmentOffset + index]; }

		inline DepthBufferType GetType() const { return m_type; }
		inline bool GetMultisampled() const { return m_multisampled; }
		inline size_t GetColorAttachmentCount() const { return m_colorAttachmentOffset; }

		unsigned int& Id() { return m_data[0]; }
		unsigned int& ColorAttachment(size_t index = 0) { return m_data[1 + index]; }
		unsigned int& DepthAttachment(size_t index = 0) { return m_data[1 + m_colorAttachmentOffset + index]; }
	};

	struct MultiTargetFrameBufferCreationSpec
	{
		unsigned int width;
		unsigned int height;
		size_t colorAttachments;
		size_t depthAttachments;
		bool floating;

		MultiTargetFrameBufferCreationSpec(unsigned int width, unsigned int height, size_t colorAttachments, 
										   size_t depthAttachments = 1, bool floating = false)
			: width(width), height(height), colorAttachments(colorAttachments), 
			  depthAttachments(depthAttachments), floating(floating)
		{
		}
	};

	struct FrameBufferCreationSpec
	{
		unsigned int width;
		unsigned int height;
		unsigned int multisampled;
		DepthBufferType depthType;
		bool floating;
		unsigned int wrapping;

		FrameBufferCreationSpec(unsigned int width, unsigned int height, unsigned int multisampled = 0,
								DepthBufferType depthtype = DepthBufferType::None, bool floating = false,
							    unsigned int wrapping = ASTRA_GL_REPEAT)
			: width(width), height(height), multisampled(multisampled), depthType(depthtype),
				floating(floating), wrapping(wrapping)
		{
		}
	};
}
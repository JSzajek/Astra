#pragma once

#include "astra_pch.h"

#include "Astra/Core.h"
#include "Astra/events/Event.h"

namespace Astra
{
	#define DEFAULT_WIDTH		960
	#define DEFAULT_HEIGHT		540
	#define DEFAULT_TITLE		"Astra Project"

	struct WindowProperties
	{
	public:
		std::string Title;
		unsigned int Width;
		unsigned int Height;
	public:
		WindowProperties(const std::string& title = DEFAULT_TITLE, unsigned int width = DEFAULT_WIDTH, unsigned int height = DEFAULT_HEIGHT)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class ASTRA_API Window
	{
	protected:
		bool m_vsync;

		// Post Processing Properties
		unsigned int m_multisampled;
		bool m_bloom;
		bool m_hdr;

		// Quality Effects
		bool m_reflections;
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() { };

		virtual void OnUpdate() = 0;
		virtual void Clear() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual std::pair<unsigned int, unsigned int> GetSize() const = 0;
		virtual float GetAspectRatio() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;

		virtual void SetMultisampling(unsigned int sampleSize) = 0;
		virtual void SetHDR(bool enabled) = 0;
		virtual void SetBloom(bool enabled) = 0;
		virtual void SetReflections(bool enabled) = 0;

		inline bool IsVSync() const { return m_vsync; }
		inline unsigned int IsMultisampling() const { return m_multisampled; }
		inline bool IsHDR() const { return m_hdr; }
		inline bool IsBloom() const { return m_bloom; }
		inline bool IsReflecting() const { return m_reflections; }

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProperties& properties = WindowProperties());
	};
}
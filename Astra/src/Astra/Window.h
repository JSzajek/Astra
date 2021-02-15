#pragma once

#include "astra_pch.h"

#include "Astra/Core.h"
#include "Astra/events/Event.h"

namespace Astra
{
	struct WindowProperties
	{
	public:
		std::string Title;
		unsigned int Width;
		unsigned int Height;
	public:
		WindowProperties(const std::string& title = "Astra Project", unsigned int width = 960, unsigned int height = 540)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class ASTRA_API Window
	{
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
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProperties& properties = WindowProperties());
	};
}
#pragma once

#include "Astra/Window.h"
#include <GLFW/glfw3.h>

namespace Astra
{
	class WindowsWindow : public Window
	{
	private:
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;

			EventCallbackFn EventCallback;
		};
	private:
		WindowData m_data;
		GLFWwindow* m_window;
	public:
		WindowsWindow(const WindowProperties& props);
		virtual ~WindowsWindow();

		virtual void OnUpdate() override;
		virtual void Clear() override;

		virtual inline unsigned int GetWidth() const override { return m_data.Width; }
		virtual inline unsigned int GetHeight() const override { return m_data.Height; }
		virtual inline std::pair<unsigned int, unsigned int> GetSize() const override {return { m_data.Width, m_data.Height }; }
		virtual inline float GetAspectRatio() const override { return static_cast<float>(m_data.Width) / static_cast<float>(m_data.Height); }

		virtual void SetEventCallback(const EventCallbackFn& callback) override { m_data.EventCallback = callback; };
		virtual void SetVSync(bool enabled) override;

		virtual void SetPostProcessing(bool enabled) override;
		virtual void SetMultisampling(unsigned int sampleSize) override;
		virtual void SetHDR(bool enabled) override;
		virtual void SetBloom(bool enabled) override;

		virtual inline void* GetNativeWindow() const override { return m_window; }
	private:
		virtual void Init(const WindowProperties& properties);
		virtual void ShutDown();
	};
}
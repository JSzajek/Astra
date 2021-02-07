#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <time.h>

#include "../math/Vec2.h"
#include "../math/Vec3.h"

#include "Input.h"

namespace Astra::Graphics
{
	#define DEFAULT_WIDTH		960
	#define DEFAULT_HEIGHT		540
	#define DEFAULT_TITLE		"Astra"

	#define V_SYNC				0	
	#define MULTI_SAMPLE		1
	#define MULTI_SAMPLE_SIZE	4
	#define HDR					1
	#define MAX_KEYS			1024
	#define MAX_BUTTONS			32

	class Window
	{
	private:
		int m_width, m_height;
		clock_t m_lastFrameTime;
		float m_delta;
		float m_last;
		const char* m_title;
		GLFWwindow* m_window;
		bool m_closed;
		std::function<void(int, int)> m_windowResizeCallback;

		std::vector<std::function<void(float, float)>> m_mouseMoveCallbacks;
	public:
		Window(const Window&) = delete;
		void operator=(const Window&) = delete;

		static Window& Get()
		{
			static Window instance;
			return instance;
		}

		inline static float GetDelta() { return Get().m_delta; }
		inline static int GetWidth() { return Get().m_width; }
		inline static int GetHeight() { return Get().m_height; }
		inline static float GetAspectRatio() { return Get().m_width / (float)Get().m_height; }
		
		static void Clear()
		{
			Get().ClearImpl();
		}

		static void Update()
		{
			Get().UpdateImpl();
		}

		static bool Closed()
		{
			return Get().ClosedImpl();
		}

		static void SetWindowSize(int width, int height)
		{
			Get().SetWindowSizeImpl(width, height);
		}

		static void SetWindowTitle(const char* title)
		{
			Get().SetWindowTitleImpl(title);
		}

		static void SetWindowResizeCallback(std::function<void(int, int)> callback)
		{
			Get().SetWindowResizeCallbackImpl(callback);
		}

	private:
		Window();
		~Window();
		void ClearImpl() const;
		void UpdateImpl();
		bool ClosedImpl() const;
		void SetWindowSizeImpl(int width, int height);
		void SetWindowTitleImpl(const char* title);
		void SetWindowResizeCallbackImpl(std::function<void(int, int)> callback);
	private:
		bool Init();
	};
} 
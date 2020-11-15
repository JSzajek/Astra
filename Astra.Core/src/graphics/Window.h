#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <time.h>

#include "../math/Vec2.h"
#include "../math/Vec3.h"

namespace Astra::Graphics
{
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
		const char* m_title;
		GLFWwindow* m_window;
		std::function<void(int, int)> m_windowResizeCallback;
		bool m_closed;

		bool m_keys[MAX_KEYS];
		bool m_mouseButtons[MAX_BUTTONS];
		Math::Vec2 m_mousePosition;
		float m_mouseScroll;

		clock_t m_lastFrameTime;
	public:
		static float delta;
		static int width, height;
	public:
		Window(const char* title, int width, int height);
		~Window();
		void Clear() const;
		void Update();
		bool Closed() const;

		inline int GetWidth() { return m_width; }
		inline int GetHeight() { return m_height; }

		void SetWindowResizeCallback(std::function<void(int, int)> callback);

		bool isKeyPressed(unsigned int keycode) const;
		bool isMouseButtonPressed(unsigned int button) const;
		const Math::Vec2& getMousePosition();
		inline float getMouseWheel() { return m_mouseScroll; }

		inline float GetDelta() { return delta; }

		static inline float GetAspectRatio() { return (float)width / float(height); }
	private:
		bool Init();
	};
} 
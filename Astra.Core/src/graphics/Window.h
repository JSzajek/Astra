#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>

#include "../math/Vec2.h"

namespace Astra::Graphics
{
	#define MAX_KEYS	1024
	#define MAX_BUTTONS 32

	class Window
	{
	private:
		int m_width, m_height;
		const char* m_title;
		GLFWwindow* m_window;
		std::function<void(float, float)> m_windowResizeCallback;
		bool m_closed;

		bool m_keys[MAX_KEYS];
		bool m_mouseButtons[MAX_BUTTONS];
		Math::Vec2 m_mousePosition;
	public:
		Window(const char* title, int width, int height);
		~Window();
		void Clear() const;
		void Update();
		bool Closed() const;

		inline int GetWidth() { return m_width; }
		inline int GetHeight() { return m_height; }

		void SetWindowResizeCallback(std::function<void(float, float)> callback);

		bool isKeyPressed(unsigned int keycode) const;
		bool isMouseButtonPressed(unsigned int button) const;
		Math::Vec2& getMousePosition();
	private:
		bool Init();
	};
} 
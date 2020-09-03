#include "Window.h"

namespace Astra::Graphics
{
	Window::Window(const char* title, int width, int height)
		: m_title(title), m_width(width), m_height(height), m_mousePosition(Math::Vec2())
	{
		if (!Init())
		{
			glfwTerminate();
		}

		for (int i = 0; i < MAX_KEYS; i++)
		{
			m_keys[i] = false;
		}

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			m_mouseButtons[i] = false;
		}
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	bool Window::Closed() const
	{
		return glfwWindowShouldClose(m_window) == 1;
	}

	bool Window::Init()
	{
		if (!glfwInit())
		{
			std::cout << "Failed to initialize glfw" << std::endl;
			return false;
		}

		m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);

		if (!m_window)
		{
			std::cout << "Failed to create window" << std::endl;
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this);
		glfwSwapInterval(0);

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->m_width = width;
			win->m_height = height;
			if (win->m_windowResizeCallback != NULL)
			{
				win->m_windowResizeCallback(width, height);
			}
			glViewport(0, 0, width, height);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scanCode, int action, int mode)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->m_keys[key] = action != GLFW_RELEASE;
		});
		
		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mode)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->m_mouseButtons[button] = action != GLFW_RELEASE;
		});
		
		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->m_mousePosition.x = (float)xPos;
			win->m_mousePosition.y = (float)yPos;
		});


		if (glewInit() != GLEW_OK)
		{
			std::cout << "Could not initialize GLEW!" << std::endl;
			return false;
		}

		std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
		return true;
	}

	void Window::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::Update()
	{
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cout << "OpenGL Error: " << error << std::endl;
		}

		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void Window::SetWindowResizeCallback(std::function<void(float, float)> callback)
	{
		m_windowResizeCallback = callback;
		m_windowResizeCallback(m_width, m_height);
	}

	bool Window::isKeyPressed(unsigned int keycode) const
	{
		//TODO:: log this
		if (keycode >= MAX_KEYS)
		{
			return false;
		}
		return m_keys[keycode];
	}

	bool Window::isMouseButtonPressed(unsigned int button) const
	{
		//TODO:: log this
		if (button >= MAX_BUTTONS)
		{
			return false;
		}
		return m_mouseButtons[button];
	}

	Math::Vec2& Window::getMousePosition()
	{
		return m_mousePosition;
	}
}
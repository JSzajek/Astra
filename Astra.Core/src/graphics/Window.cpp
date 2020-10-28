#include "Window.h"
#include "../logger/Logger.h"

namespace Astra::Graphics
{
	double Window::delta;
	int Window::width;
	int Window::height;
	
	Window::Window(const char* title, int width, int height)
		: m_title(title), m_width(width), m_height(height), m_mousePosition(Math::Vec2()),
			m_mouseScroll(0)
	{
		if (!Init())
		{
			glfwTerminate();
		}
	
		this->width = m_width;
		this->height = m_height;

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
			Logger::LogError("Failed to Initialize GLFW.");
			return false;
		}

		m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);

		if (!m_window)
		{
			Logger::LogError("Failed to Create Window Instance.");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this);
	
	#if MULTI_SAMPLE
		glfwWindowHint(GLFW_SAMPLES, 4);
		glEnable(GL_MULTISAMPLE);
	#endif

	#if V_SYNC
		glfwSwapInterval(0);
	#endif

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->width = win->m_width = width;
			win->height = win->m_height = height;
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

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->m_mouseScroll = (float)yOffset;
		});


		if (glewInit() != GLEW_OK)
		{
			Logger::LogError("Failed to Initialize GLEW." );
			return false;
		}

		m_lastFrameTime = glfwGetTime();

		Logger::Log(std::string("OpenGL ") + std::string((const char*)glGetString(GL_VERSION)));
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
			Logger::LogError(std::string("OpenGL Error: ") + std::to_string(error));
			Logger::LogError((const char*)(glewGetErrorString(error)));
		}

		const double& currentFrameTime = glfwGetTime();
		delta = (currentFrameTime - m_lastFrameTime);
		m_lastFrameTime = currentFrameTime;
		m_mouseScroll = 0;

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
		if (keycode >= MAX_KEYS)
		{
			Logger::LogWarning("Detected key input outside of MAX_KEYS bounds.");
			return false;
		}
		return m_keys[keycode];
	}

	bool Window::isMouseButtonPressed(unsigned int button) const
	{
		if (button >= MAX_BUTTONS)
		{
			Logger::LogWarning("Detected mouse button input outside of MAX_BUTTONS bounds.");
			return false;
		}
		return m_mouseButtons[button];
	}

	const Math::Vec2& Window::getMousePosition()
	{
		return m_mousePosition;
	}
}
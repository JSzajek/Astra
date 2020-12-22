#include "Window.h"
#include "../logger/Logger.h"

#include <time.h>
namespace Astra::Graphics
{
	Window::Window()
		: m_title(DEFAULT_TITLE), m_width(DEFAULT_WIDTH), m_height(DEFAULT_HEIGHT)
	{
		if (!Init())
		{
			glfwTerminate();
		}
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	bool Window::ClosedImpl() const
	{
		return glfwWindowShouldClose(m_window) == 1;
	}

	void Window::ClearImpl() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::UpdateImpl()
	{
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			Logger::LogError(std::string("OpenGL Error: ") + std::to_string(error));
			Logger::LogError((const char*)(glewGetErrorString(error)));
		}

		auto currentTick = clock();
		m_delta = ((float)(currentTick - m_lastFrameTime)) / CLOCKS_PER_SEC;
		m_lastFrameTime = currentTick;

		Input::Flush();
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
	
	void Window::SetWindowSizeImpl(int width, int height)
	{
		m_width = width;
		m_height = height;
		glfwSetWindowSize(m_window, m_width, m_height);
	}

	void Window::SetWindowTitleImpl(const char* title)
	{
		m_title = title;
		glfwSetWindowTitle(m_window, m_title);
	}

	void Window::SetWindowResizeCallbackImpl(std::function<void(int, int)> callback)
	{
		m_windowResizeCallback = callback;
		m_windowResizeCallback(m_width, m_height);
	}

	bool Window::Init()
	{
		if (!glfwInit())
		{
			Logger::LogError("Failed to Initialize GLFW.");
			return false;
		}

		if (!glewIsSupported("GL_VERSION_3_3"))
		{
			Logger::LogError("OpenGL Version 3.3 Not Supported.");
			return false;
		}

		// Force usage of OpenGL 3.3.0
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);

		if (!m_window)
		{
			Logger::LogError("Failed to Create Window Instance.");
			glfwTerminate();
			return false;
		}

		Input::Init(m_window);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this);
	
	#if MULTI_SAMPLE
		glEnable(GL_MULTISAMPLE);
	#endif

	#if V_SYNC
		glfwSwapInterval(0);
	#endif

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

		if (glewInit() != GLEW_OK)
		{
			Logger::LogError("Failed to Initialize GLEW." );
			return false;
		}

		m_lastFrameTime = clock();

		Logger::Log(std::string("OpenGL ") + std::string((const char*)glGetString(GL_VERSION)));
		return true;
	}
}
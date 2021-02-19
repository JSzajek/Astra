#include "astra_pch.h"

#include "WindowsWindow.h"

#include "Astra/events/ApplicationEvent.h"
#include "Astra/events/KeyEvent.h"
#include "Astra/events/MouseEvent.h"

#include <GL/glew.h>

#include "Astra/Application.h" // For vsync define - move?
#include "Astra/Scene.h"

namespace Astra
{
	static bool GLFWInitialized = false;
	static bool GLEWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		ASTRA_CORE_ERROR("GLFW ERROR: [{0}]: {1}", error, description);
	}
	
	Window* Window::Create(const WindowProperties& properties)
	{
		return new WindowsWindow(properties);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& properties)
	{
		m_vsync = false;
		m_multisampled = 0; 
		m_bloom = false;
		m_hdr = false;

		Init(properties);
	}

	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}

	void WindowsWindow::Init(const WindowProperties& properties)
	{
		m_data.Title = properties.Title;
		m_data.Width = properties.Width;
		m_data.Height = properties.Height;

		ASTRA_CORE_INFO("Intializing Window {0} ({1}, {2})", m_data.Title, m_data.Width, m_data.Height);

		if (!GLFWInitialized)
		{
			int success = glfwInit();
			ASTRA_CORE_ASSERT(success, "GLFW Couldn't initialize.");

			glfwSetErrorCallback(GLFWErrorCallback);

			GLFWInitialized = true;
		}

		// Force usage of OpenGL 3.3.0
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		m_window = glfwCreateWindow((int)properties.Width, (int)properties.Height, properties.Title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_data);

		SetMultisampling(m_multisampled);
		SetVSync(m_vsync);

		if (!GLEWInitialized)
		{
			auto success = glewInit();
			ASTRA_CORE_ASSERT(success == GLEW_OK, "GLEW Failed to Initialize.");
			GLEWInitialized = true;
		}

		ASTRA_CORE_INFO("OpenGL {0}", std::string((const char*)glGetString(GL_VERSION)));

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = static_cast<unsigned int>(width);
			data.Height = static_cast<unsigned int>(height);
			
			WindowResizeEvent _event(width, height);
			data.EventCallback(_event);
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent _event;
			data.EventCallback(_event);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			if (action == GLFW_PRESS)
			{
				KeyPressedEvent _event(key, 0);
				data.EventCallback(_event);
			}
			else if (action == GLFW_RELEASE)
			{
				KeyReleasedEvent _event(key);
				data.EventCallback(_event);
			}
			else if (action == GLFW_REPEAT)
			{
				KeyPressedEvent _event(key, 1);
				data.EventCallback(_event);
			}
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			if (action == GLFW_PRESS)
			{
				MouseButtonPressedEvent _event(button);
				data.EventCallback(_event);
			}
			else if (action == GLFW_RELEASE)
			{
				MouseButtonReleasedEvent _event(button);
				data.EventCallback(_event);
			}
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent _event(static_cast<float>(xOffset), static_cast<float>(yOffset));
			data.EventCallback(_event);
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent _event(static_cast<float>(xPos), static_cast<float>(yPos));
			data.EventCallback(_event);
		});
	}

	void WindowsWindow::ShutDown()
	{
		glfwDestroyWindow(m_window);
	}

	void WindowsWindow::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		m_vsync = enabled;
		glfwSwapInterval(m_vsync);
	}

	void WindowsWindow::SetMultisampling(unsigned int sampleSize)
	{
		m_multisampled = sampleSize;
		if (m_multisampled > 0)
		{
			glEnable(GL_MULTISAMPLE);
		}
		else
		{
			glDisable(GL_MULTISAMPLE);
		}

		auto* scene = Application::Get().GetCurrentScene();
		if (scene)
			((Astra::Scene*)scene)->SetMultisampling(m_multisampled);
	}

	void WindowsWindow::SetHDR(bool enabled)
	{
		m_hdr = enabled;
		auto* scene = Application::Get().GetCurrentScene();
		if (scene)
			((Astra::Scene*)scene)->SetHDR(m_hdr);
	}

	void WindowsWindow::SetBloom(bool enabled)
	{
		m_bloom = enabled;
		auto* scene = Application::Get().GetCurrentScene();
		if (scene)
			((Astra::Scene*)scene)->SetBloom(m_bloom);
	}
}
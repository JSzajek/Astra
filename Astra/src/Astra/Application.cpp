#include "astra_pch.h"

#include "Application.h"
#include "Input.h"

#include <GLFW\glfw3.h>

#include "Astra/graphics/renderers/RendererController.h"

namespace Astra
{
	#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_instance = NULL;

	Application::Application()
	{
		ASTRA_CORE_ASSERT(!s_instance, "Application already exists.");

		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_layerStack.PushOverlay(layer);
	}

	void Application::Run()
	{
		while(m_running) 
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_lastFrameTime;
			m_lastFrameTime = time;

			if (!m_minimized)
			{
				for (Layer* layer : m_layerStack)
				{
					layer->OnUpdate(timestep);
				}
			}
			m_window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& _event)
	{
		EventDispatcher dispatcher(_event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
		{
			(*--it)->OnEvent(_event);
			if (_event.Handled)
			{
				break;
			}
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& _event)
	{
		m_running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& _event)
	{
		if (_event.GetWidth() == 0 || _event.GetHeight() == 0)
		{
			m_minimized = true;
			return false;
		}
		m_minimized = false;

		// Connect Renderer Resize here
		Astra::Graphics::RendererController::UpdateScreen(_event.GetWidth(), _event.GetHeight());

		return true;
	}
}
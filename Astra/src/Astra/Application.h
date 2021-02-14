#pragma once

#include "Core.h"
#include "Window.h"
#include "Astra/LayerStack.h"
#include "Astra/utils/Timestep.h"
#include "Astra/events/ApplicationEvent.h"

namespace Astra
{
	#define DEFAULT_WIDTH		960
	#define DEFAULT_HEIGHT		540
	#define DEFAULT_TITLE		"Astra"

	#define V_SYNC				0	
	#define MULTI_SAMPLE		1
	#define MULTI_SAMPLE_SIZE	4
	#define BLOOM				1
	#define HDR					1
	#define MAX_KEYS			1024
	#define MAX_BUTTONS			32

	class ASTRA_API Application
	{
	private:
		static Application* s_instance;
	private:
		std::unique_ptr<Window> m_window;
		bool m_running = true;
		bool m_minimized = false;
		LayerStack m_layerStack;
		float m_lastFrameTime = 0.0f;
	public:
		Application();
		virtual ~Application();
		
		void Run();

		void OnEvent(Event& _event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		static Application& Get() { return *s_instance; }
		inline Window& GetWindow() { return *m_window; }
	private:
		bool OnWindowClose(WindowCloseEvent& _event);
		bool OnWindowResize(WindowResizeEvent& _event);
	};

	// To be defined in client
	Application* CreateApplication();
}
#pragma once

#include "Core.h"
#include "Window.h"
#include "Astra/layers/LayerStack.h"
#include "Astra/utils/Timestep.h"
#include "Astra/events/ApplicationEvent.h"

int main(int argc, char** argv); // Global entry point definition

namespace Astra
{
	#define V_SYNC				0	
	#define MULTI_SAMPLE		0
	#define MULTI_SAMPLE_SIZE	4
	#define BLOOM				1
	#define HDR					1

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

		void OnEvent(Event& _event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		
		inline void PopLayer(Layer* layer) { m_layerStack.PopLayer(layer); }
		inline void PopOverlay(Layer* layer) { m_layerStack.PopOverlay(layer); }

		virtual void OnUpdate(float delta) { }

		static Application& Get() { return *s_instance; }
		inline Window& GetWindow() { return *m_window; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& _event);
		bool OnWindowResize(WindowResizeEvent& _event);

		friend int ::main(int argc, char** argv);
	};

	// To be defined in client
	Application* CreateApplication();
}
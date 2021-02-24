#pragma once

#include "Core.h"
#include "Window.h"
#include "Astra/layers/LayerStack.h"
#include "Astra/utils/Timestep.h"
#include "Astra/events/ApplicationEvent.h"

int main(int argc, char** argv); // Global entry point definition

namespace Astra
{
	class ASTRA_API Application
	{
	private:
		static Application* s_instance;
	private:
		void* m_currentScene;
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

		virtual void OnAwake() { }
		virtual void OnUpdate(float delta) { }

		static Application& Get() { return *s_instance; }
		inline Window& GetWindow() { return *m_window; }

		inline void SetCurrentScene(void* pointer) { m_currentScene = pointer; }
		inline void* GetCurrentScene() const { return m_currentScene; }
	private:
		void Run();

		bool OnWindowClose(WindowCloseEvent& _event);
		bool OnWindowResize(WindowResizeEvent& _event);

		friend int ::main(int argc, char** argv);
	};

	// To be defined in client
	Application* CreateApplication();
}
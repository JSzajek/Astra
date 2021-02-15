#pragma once
#include "Layer.h"

#include "Astra/graphics/renderers/GuiRenderer.h"

#include "Astra/graphics/guis/Gui.h"

#include "Astra/events/KeyEvent.h"
#include "Astra/events/MouseEvent.h"

namespace Astra
{
	class Layer2D : public Layer
	{
	private:
		Graphics::Gui* m_focused;
	private:
		// Renderer
		Graphics::GuiRenderer* m_guiRenderer;
		
		// Guis
		std::vector<std::tuple<Graphics::Gui*, int>> m_guis;
	public:
		Layer2D();
		~Layer2D();
	public:
		inline void AddGui(Graphics::Gui* gui, int layer) { m_guis.push_back(std::make_tuple(gui, layer)); }
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float delta) override;
		virtual void OnEvent(Event& _event) override;
		virtual void UpdateScreen(unsigned int width, unsigned int height) override;
	private:
		void Render();
	private:
		bool OnMouseMoved(MouseMovedEvent& _event);
		bool OnMousePressed(MouseButtonPressedEvent& _event);
		bool OnMouseReleased(MouseButtonReleasedEvent& _event);
	};
}
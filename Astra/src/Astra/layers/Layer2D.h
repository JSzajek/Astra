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
		
		// Loaded Guis
		std::unordered_map<unsigned int, Graphics::Button> m_buttons;
		std::unordered_map<unsigned int, Graphics::ToggleButton> m_toggleButtons;
		std::unordered_map<unsigned int, Graphics::Panel> m_panels;
		std::unordered_map<unsigned int, Graphics::Label> m_labels;
		std::unordered_map<unsigned int, Graphics::Image> m_images;
		std::unordered_map<unsigned int, Graphics::TextBox> m_textboxes;

		std::unordered_map<std::string, Graphics::Gui*> m_loaded;
		std::vector<Graphics::GuiLayer> m_layers;

		// Guis
		std::vector<std::tuple<Graphics::Gui*, int>> m_guis;
	public:
		Layer2D();
		~Layer2D();
	public:
		Graphics::LayerEntity* Get(std::string name);

		Graphics::Button* Add(const Graphics::Button& gui, int layer);
		Graphics::ToggleButton* Add(const Graphics::ToggleButton& gui, int layer);
		Graphics::Panel* Add(const Graphics::Panel& gui, int layer);
		Graphics::Label* Add(const Graphics::Label& gui, int layer);
		Graphics::Image* Add(const Graphics::Image& gui, int layer);
		Graphics::TextBox* Add(const Graphics::TextBox& gui, int layer);
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float delta) override;
		virtual void OnEvent(Event& _event) override;
		virtual void UpdateScreen(unsigned int width, unsigned int height) override;
	private:
		void Render();
		
		template<class T>
		T* AddGui(Graphics::Gui* gui, int layer);
	private:
		bool OnMouseMoved(MouseMovedEvent& _event);
		bool OnMousePressed(MouseButtonPressedEvent& _event);
		bool OnMouseReleased(MouseButtonReleasedEvent& _event);
	};
}
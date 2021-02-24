#include "astra_pch.h"

#include "Layer2D.h"
#include "Astra/Application.h"

#include "Astra/graphics/shaders/GuiShader.h"
#include "Astra/graphics/shaders/FontShader.h"

#include "Astra/inputs/MouseCodes.h"
#include "Astra/inputs/Input.h"

namespace Astra
{
	Layer2D::Layer2D()
		: m_focused(NULL)
	{
		m_guiRenderer = new Graphics::GuiRenderer(new Graphics::GuiShader(), new Graphics::FontShader());
	}
	
	Layer2D::~Layer2D()
	{
	}

	Graphics::LayerEntity* Layer2D::Get(std::string name)
	{
		if (m_loaded.find(name) != m_loaded.end())
		{
			return m_loaded[name];
		}
		return NULL;
	}

	void Layer2D::OnAttach()
	{
		// Update Projection Matrix
		auto [width, height] = Application::Get().GetWindow().GetSize();
		UpdateScreen(width, height);
	}
	
	void Layer2D::OnDetach()
	{
	}
	
	void Layer2D::OnUpdate(float delta)
	{
		Render();
	}
	
	void Layer2D::OnEvent(Event& _event)
	{
		EventDispatcher dispatcher(_event);
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(Layer2D::OnMouseMoved));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(Layer2D::OnMousePressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(Layer2D::OnMouseReleased));
	}

	void Layer2D::UpdateScreen(unsigned int width, unsigned int height)
	{
		// Keeping the original design size causes the gui to scale up or down based on the window.
		// This could be potentially counteractive for higher scale resizing causing the loss of resolution.
		// TODO :: Add switch to either scale up implicitly or keep same resolution.
		auto ortho = Math::Mat4::Orthographic(0, 960, 540, 0, -1, 1);
		m_guiRenderer->UpdateProjectionMatrix(&ortho);
	}

	void Layer2D::Render()
	{
		m_guiRenderer->Draw(m_layers);
	}
	
	template<class T>
	T* Layer2D::AddGui(Graphics::Gui* gui, int level)
	{
		for (auto& layer : m_layers)
		{
			if (layer.GetLevel() == level)
			{
				layer.Add(gui);
				return (T*)gui;
			}
		}

		Graphics::GuiLayer newLayer(level);
		newLayer.Add(gui);
		m_layers.push_back(newLayer);

		// Insertion Sort layers
		for (auto it = m_layers.begin(); it != m_layers.end(); it++)
		{
			auto const insertion_point = std::upper_bound(m_layers.begin(), it, *it,
				[](const Graphics::GuiLayer& a, const Graphics::GuiLayer& b) {return b.GetLevel() > a.GetLevel(); });
			std::rotate(insertion_point, it, it + 1);
		}
		return (T*)gui;
	}

	bool Layer2D::OnMouseMoved(MouseMovedEvent& _event)
	{
		auto position = Math::Vec2(_event.GetX(), _event.GetY());
		if (m_focused && !m_focused->GetBounds().HasPoint(position))
		{
			m_focused->OnExit();
			m_focused = NULL;
		}

		for (const auto& layer : m_layers)
		{
			// TODO: deal with click through or passing the event along. For now click through textboxes
			/*for (auto* gui : layer.GetTexts())
			{
				if (gui->GetBounds().HasPoint(position))
				{
					m_focused = gui;
					gui->OnHover();
					return true;
				}
			}*/

			for (auto* gui : layer.GetDefaultGuis())
			{
				if (gui->GetBounds().HasPoint(position))
				{
					m_focused = gui;
					gui->OnHover();
					return true;
				}
			}

			for (auto* gui : layer.GetCustomGuis())
			{
				if (gui->GetBounds().HasPoint(position))
				{
					m_focused = gui;
					gui->OnHover();
					return true;
				}
			}
		}
		return true;
	}

	bool Layer2D::OnMousePressed(MouseButtonPressedEvent& _event)
	{
		auto _e = static_cast<MouseButtonPressedEvent&>(_event);
		if (m_focused)
		{
			if (_e.GetMouseButton() == Mouse::Button0)
			{
				m_focused->OnPressed();
			}
		}
		return true;
	}

	bool Layer2D::OnMouseReleased(MouseButtonReleasedEvent& _event)
	{
		auto [x, y] = Input::GetMousePosition();
		auto position = Math::Vec2(x, y);
		if (m_focused)
		{
			auto inBounds = m_focused->GetBounds().HasPoint(position);
			m_focused->OnReleased(inBounds);
			if (!inBounds)
			{
				m_focused = NULL;
			}
		}
		return true;
	}

	Graphics::Button* Layer2D::Add(const Graphics::Button& gui, int layer)
	{
		auto uid = gui.GetUID();
		m_buttons[uid] = gui;
		m_loaded[gui.ToString()] = &m_buttons[uid];
		return AddGui<Graphics::Button>(&m_buttons[uid], layer);
	}

	Graphics::ToggleButton* Layer2D::Add(const Graphics::ToggleButton& gui, int layer)
	{
		auto uid = gui.GetUID();
		m_toggleButtons[uid] = gui;
		m_loaded[gui.ToString()] = &m_toggleButtons[uid];
		return AddGui<Graphics::ToggleButton>(&m_toggleButtons[uid], layer);
	}

	Graphics::Panel* Layer2D::Add(const Graphics::Panel& gui, int layer)
	{
		auto uid = gui.GetUID();
		m_panels[uid] = gui;
		m_loaded[gui.ToString()] = &m_panels[uid];
		return AddGui<Graphics::Panel>(&m_panels[uid], layer);
	}

	Graphics::Label* Layer2D::Add(const Graphics::Label& gui, int layer)
	{
		auto uid = gui.GetUID();
		m_labels[uid] = gui;
		m_loaded[gui.ToString()] = &m_labels[uid];
		return AddGui<Graphics::Label>(&m_labels[uid], layer);
	}

	Graphics::Image* Layer2D::Add(const Graphics::Image& gui, int layer)
	{
		auto uid = gui.GetUID();
		m_images[uid] = gui;
		m_loaded[gui.ToString()] = &m_images[uid];
		return AddGui<Graphics::Image>(&m_images[uid], layer);
	}

	Graphics::TextBox* Layer2D::Add(const Graphics::TextBox& gui, int layer)
	{
		auto uid = gui.GetUID();
		m_textboxes[uid] = gui;
		m_loaded[gui.ToString()] = &m_textboxes[uid];
		return AddGui<Graphics::TextBox>(&m_textboxes[uid], layer);
	}
}
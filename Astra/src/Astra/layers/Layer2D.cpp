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

	void Layer2D::OnAttach()
	{
		for (const auto& gui : m_guis)
		{
			m_guiRenderer->AddGui(std::get<0>(gui), std::get<1>(gui));
		}

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
		m_guiRenderer->Draw();
	}

	bool Layer2D::OnMouseMoved(MouseMovedEvent& _event)
	{
		auto position = Math::Vec2(_event.GetX(), _event.GetY());
		if (m_focused && !m_focused->GetBounds().HasPoint(position))
		{
			m_focused->OnExit();
			m_focused = NULL;
		}

		for (auto tuple_gui : m_guis)
		{
			auto* gui = std::get<0>(tuple_gui);
			if (gui->GetBounds().HasPoint(position))
			{
				m_focused = gui;
				gui->OnHover();
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
}
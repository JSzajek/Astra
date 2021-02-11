#include "renderers/RendererController.h" // Move to event system?

#include "Input.h"


namespace Astra
{
	Input::Input()
		: m_window(NULL), m_mouseScroll(0), m_mousePosition(Math::Vec2()),
			m_pressed(0), m_released(0)
	{
	}

	void Input::InitImpl(GLFWwindow* ptr)
	{
		m_window = ptr;

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			Input::SetMousePosition(static_cast<float>(xPos), static_cast<float>(yPos));
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			Input::SetMouseScroll(static_cast<float>(yOffset));
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scanCode, int action, int mode)
		{
			if (action == GLFW_PRESS)
			{
				Input::SetPressedKey(key);
			}
			else if (action == GLFW_RELEASE)
			{
				Input::SetReleasedKey(key);
			}
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			if (action == GLFW_PRESS)
			{
				Input::SetMousePressedKey(button);
			}
			else if (action == GLFW_RELEASE)
			{
				Input::SetMouseReleasedKey(button);
			}
		});

	}

	void Input::FlushImpl()
	{
		m_mouseScroll = 0;
		m_pressed = 0;
		m_released = 0;
	}

	bool Input::IsKeyPressedImpl(unsigned int key) const
	{
		return glfwGetKey(m_window, key) == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressedImpl(unsigned int button) const
	{
		return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
	}

	void Input::SetMousePressedKeyImpl(unsigned int key)
	{
		m_mousePressed = key;
		if (m_mousePressed == GLFW_MOUSE_BUTTON_1)
		{
			Astra::Graphics::RendererController::CheckInput(m_mousePosition, GLFW_PRESS);
		}
	}

	void Input::SetMouseReleasedKeyImpl(unsigned int key)
	{
		m_mousePressed = key;
		if (m_mousePressed == GLFW_MOUSE_BUTTON_1)
		{
			Astra::Graphics::RendererController::CheckInput(m_mousePosition, GLFW_RELEASE);
		}
	}
}
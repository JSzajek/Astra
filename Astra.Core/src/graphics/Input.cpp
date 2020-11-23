#include "Input.h"

namespace Astra
{
	Input::Input()
		: m_window(NULL), m_mouseScroll(0), m_mousePosition(Math::Vec2())
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
	}

	void Input::FlushImpl()
	{
		m_mouseScroll = 0;
	}

	bool Input::IsKeyPressedImpl(unsigned int key) const
	{
		return glfwGetKey(m_window, key) == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressedImpl(unsigned int button) const
	{
		return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
	}
}
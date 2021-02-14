#include "astra_pch.h"

#include "WindowsInput.h"
#include "Astra/Application.h"

#include <GLFW/glfw3.h>

namespace Astra
{
	Input* Input::s_instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { (float)xPos, (float)yPos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [xPos, yPos] = GetMousePositionImpl();
		return xPos;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [xPos, yPos] = GetMousePositionImpl();
		return yPos;
	}
}
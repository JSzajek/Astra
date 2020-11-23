#pragma once

#include <GLFW/glfw3.h>

#include "../math/Vec2.h"
#include "../math/Vec3.h"

#include "Inputs.h"

namespace Astra
{
	class Input
	{
	private:
		GLFWwindow* m_window;
		float m_mouseScroll;
		Math::Vec2 m_mousePosition;
	public:
		Input(const Input&) = delete;
		void operator=(const Input&) = delete;

		static Input& Get()
		{
			static Input instance;
			return instance;
		}

		static void Init(GLFWwindow* ptr)
		{
			Get().InitImpl(ptr);
		}

		static void Flush()
		{
			return Get().FlushImpl();
		}

		static bool IsKeyPressed(Key key)
		{
			return Get().IsKeyPressedImpl(static_cast<unsigned int>(key));
		}

		static bool IsMouseButtonPressed(Mouse mouse)
		{
			return Get().IsMouseButtonPressedImpl(static_cast<unsigned int>(mouse));
		}

		inline static void SetMousePosition(float x, float y) { Get().SetMousePositionImpl(x,y); }
		inline static void SetMouseScroll(float scroll) { Get().SetMouseScrollImpl(scroll); }

		inline static const Math::Vec2& GetMousePosition() { return Get().m_mousePosition; }
		inline static float GetMouseScroll() { return Get().m_mouseScroll; }

	protected:
	private:
		Input();
		void InitImpl(GLFWwindow* ptr);
		void FlushImpl();
		bool IsKeyPressedImpl(unsigned int key) const;
		bool IsMouseButtonPressedImpl(unsigned int button) const;
		inline void SetMousePositionImpl(float x, float y) { m_mousePosition.x = x; m_mousePosition.y = y; };
		inline void SetMouseScrollImpl(float scroll) { m_mouseScroll = scroll; };
	};


}
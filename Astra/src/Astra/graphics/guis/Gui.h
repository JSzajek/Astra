#pragma once

#include <functional>

#include "../../math/Vec2.h"
#include "Spatial2D.h"
#include "utility/Rect2.h"

#include "../materials/GuiMaterial.h"
#include "Color.h"

#include "utility/FontAtlas.h"

namespace Astra::Graphics
{
	enum class GuiType : unsigned char
	{
		Panel,
		Label,
		Button,
		Image,
		TextBox
	};

	class Gui : public Spatial2D
	{
	private:
		GuiType m_type;
	protected:
		Color m_modulate;
		std::function<void()> m_onHover;
		std::function<void()> m_onExit;
		std::function<void()> m_onPressed;
		std::function<void()> m_onReleased;
		Rect2 m_rect;
	public:
		std::string Name;
		const GuiMaterial* Material;
	public:
		Gui(const GuiMaterial* material, const Math::Vec2& position, float rotation, const Math::Vec2& scale);
		Gui(const GuiMaterial* material, const char* const name, const Math::Vec2& position, float rotation, const Math::Vec2& scale);
		Gui(const Gui& other);
		~Gui();

		inline void SetModulate(const Color& modulate) { m_modulate = modulate; }
		inline virtual const Color& GetModulate() const { return m_modulate; }
		
		virtual inline const bool HasCustomVao() const { return false; }
		virtual inline const int GetCustomVao() const { return -1; }
		virtual inline const int GetCustomVbo() const { return -1; }

		virtual inline const Math::Vec2& GetSize() const { return Material->GetSize(); }
		
		inline const GuiType& GetType() const { return m_type; }
	
		inline const Rect2& GetBounds() { return m_rect; }

		inline void SetOnHover(std::function<void()> func) { m_onHover = func; }
		inline void SetOnExit(std::function<void()> func) { m_onExit = func; }
		inline void SetOnPressed(std::function<void()> func) { m_onPressed = func; }
		inline void SetOnReleased(std::function<void()> func) { m_onReleased = func; }

		inline virtual void OnHover() { if (m_onHover) m_onHover(); };
		inline virtual void OnExit() { if (m_onExit) m_onExit(); };
		inline virtual void OnPressed() { if (m_onPressed) m_onPressed(); };
		inline virtual void OnReleased() { if (m_onReleased) m_onReleased(); };
	protected:
		inline void SetType(GuiType type) { m_type = type; }
	};
}
#pragma once

#include "../../math/Vec2.h"
#include "../materials/GuiMaterial.h"

namespace Astra::Graphics
{
	class Gui
	{
	public:
		std::string Name;
		Math::Vec2 Position;
		float Rotation;
		Math::Vec2 Scale;
		const GuiMaterial* Material;
	public:
		Gui(const GuiMaterial* material, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
			: Material(material), Position(position), Rotation(rotation), Scale(scale)
		{
		}
		
		Gui(const GuiMaterial* material, const char* const name, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
			: Gui(material, position, rotation, scale)
		{
			Name = name;
		}
	};
}
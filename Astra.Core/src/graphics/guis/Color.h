#pragma once

#include <string>

#include "../../logger/Logger.h"

#include "../../math/Vec4.h"

namespace Astra::Graphics
{
	struct Color
	{
	public:
		static const Color White;
		static const Color Black;
		static const Color Blue;
		static const Color Red;
		static const Color Green;
		static const Color Yellow;
		static const Color Pink;
		static const Color Purple;
	private:
		Math::Vec4 m_data;
	public:
		Color()
			: m_data(1)
		{
		}

		Color(float r, float g, float b, float a = 1)
			: m_data(r, g, b, a)
		{
		}

		Color(const char* const hexcode)
		{
			if (strlen(hexcode) != 7)
			{
				Logger::LogWarning("Color Error: Incorrect Hexcode Length - Defaulting to Black");
				m_data = Math::Vec4(0);
				return;
			}

			std::string code(hexcode);
			m_data = Math::Vec4(std::stoi(code.substr(1, 2), 0, 16) / 255.0f, 
								std::stoi(code.substr(3, 2), 0, 16) / 255.0f, 
								std::stoi(code.substr(5, 2), 0, 16) / 255.0f, 1);
		}

		operator Math::Vec4() const { return m_data; }

		inline float GetR() { return m_data.x; }
		inline float GetG() { return m_data.y; }
		inline float GetB() { return m_data.z; }
		inline float GetA() { return m_data.w; }
	};
}
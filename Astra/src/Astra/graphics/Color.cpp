#include "astra_pch.h"

#include "Color.h"

namespace Astra::Graphics
{
	const Color Color::White	= Color(1, 1, 1);
	const Color Color::Black	= Color(0, 0, 0);
	const Color Color::Blue		= Color(0, 0, 1);
	const Color Color::Red		= Color(1, 0, 0);
	const Color Color::Green	= Color(0, 1, 0);
	const Color Color::Yellow	= Color(1, 1, 0);

	const Color Color::Pink		= Color("#FFC0CB");
	const Color Color::Purple	= Color("#800080");

	Color& Color::Multiply(const Color& other)
	{
		m_data.x *= other.GetR();
		m_data.y *= other.GetG();
		m_data.z *= other.GetB();
		m_data.w *= other.GetA();
		return *this;
	}

	Color& operator*(Color left, const Color& right)
	{
		return left.Multiply(right);
	}
}
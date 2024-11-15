#pragma once

#include <Windows.h>

namespace Astra
{
	class Timer
	{
	private:
		LARGE_INTEGER m_start;
		double m_frequency;
	public:
		Timer()
		{
			LARGE_INTEGER  frequency;
			QueryPerformanceFrequency(&frequency);
			m_frequency = 1.0 / frequency.QuadPart;
			QueryPerformanceCounter(&m_start);
		}

		void Reset()
		{
			QueryPerformanceCounter(&m_start);
		}

		float Elapsed()
		{
			LARGE_INTEGER current;
			QueryPerformanceCounter(&current);
			unsigned __int64 cycles = current.QuadPart - m_start.QuadPart;
			return (float)(cycles * m_frequency);
		}
	};
}
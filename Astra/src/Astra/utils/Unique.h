#pragma once

namespace Astra
{
	// TODO - Switch to using actual UUID or GUID generation
	static size_t GenerateUniqueId()
	{
		static size_t current = 0;
		return current++;
	}
}
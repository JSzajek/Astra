#pragma once

#include <string>
#include "Astra/utils/Unique.h"

namespace Astra::Graphics
{
	struct LayerEntity
	{
	protected:
		size_t m_uid;
	public:
		std::string Name;
	public:
		LayerEntity()
			: Name(), m_uid(GenerateUniqueId())
		{
		}

		LayerEntity(const char* const name)
			: Name(name), m_uid(GenerateUniqueId())
		{
		}

		LayerEntity(const LayerEntity& copy)
			: Name(copy.Name), m_uid(copy.m_uid)
		{
		}

		virtual std::string ToString() const = 0;
		inline unsigned int GetUID() const { return m_uid; }
	};
}
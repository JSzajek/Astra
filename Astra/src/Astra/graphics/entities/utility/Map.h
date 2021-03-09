#pragma once

#include "SimpStructs.h"

namespace Astra::Graphics
{
	#define P	131313

	class Map
	{
	private:
		Face table[2 * MAX_FACE];
		bool fill[2 * MAX_FACE] = {};
		bool lazy[2 * MAX_FACE] = {};
	public:
		Map() = default;

		unsigned int Hash(const Face& face)
		{
			unsigned long long int a1 = face.indices[0] + face.indices[1] + face.indices[2];
			unsigned long long int a2 = face.indices[0] * face.indices[1] + face.indices[1] * face.indices[2] + face.indices[2] * face.indices[0];
			unsigned long long int a3 = face.indices[0] * face.indices[1] * face.indices[2];
			unsigned long long int temp = (a1 * P + a2) * P + a3;
			return (unsigned int)(temp % (int)(MAX_FACE));
		}

		void Insert(const Face& face)
		{
			auto hash = Hash(face);
			unsigned int step = 0;
			auto position = hash;

			while (fill[position])
			{
				step++;
				position = (hash + step * step) % (int)(MAX_FACE);
			}
			table[position] = face;
			fill[position] = true;
		}

		bool Get(const Face& face, Face& real)
		{
			auto hash = Hash(face);
			unsigned int step = 0;
			auto position = hash;

			while ((fill[position] && (!(table[position] == face))) || (!fill[position] && lazy[position]))
			{
				++step;
				position = (hash + step * step) % (int)(MAX_FACE);
			}

			if (fill[position])
			{
				real = table[position];
				return true;
			}
			return false;
		}

		bool Remove(const Face& face)
		{
			auto hash = Hash(face);
			unsigned int step = 0;
			auto position = hash;
			
			while ((fill[position] && (!(table[position] == face))) || (!fill[position] && lazy[position]))
			{
				++step;
				position = (hash + step * step) % (int)(MAX_FACE);
			}

			if (fill[position])
			{
				fill[position] = false;
				lazy[position] = true;
				return true;
			}
			return false;
		}
	};
}
#pragma once

#include "Astra/math/Mat4.h"

namespace Astra::Graphics
{
	struct BoneInfo
	{
		int Id;
		Math::Mat4 Offset;

		BoneInfo() = default;
		BoneInfo(int id, const Math::Mat4& offset)
			: Id(id), Offset(offset)
		{
		}
	};
}
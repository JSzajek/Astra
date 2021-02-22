#pragma once

#include <assimp/anim.h>

#include "Astra/math/Mat4.h"
#include "Astra/math/Vec3.h"
#include "Astra/math/Quat.h"

namespace Astra::Graphics
{
	static Math::Mat4 ConvertAiMatrix(const aiMatrix4x4& mat)
	{
		// TODO: Improve Mat4 class to allow for explicit initialization.
		Math::Mat4 converted;
		converted.columns[0][0] = mat.a1;
		converted.columns[0][1] = mat.b1;
		converted.columns[0][2] = mat.c1;
		converted.columns[0][3] = mat.d1;

		converted.columns[1][0] = mat.a2;
		converted.columns[1][1] = mat.b2;
		converted.columns[1][2] = mat.c2;
		converted.columns[1][3] = mat.d2;

		converted.columns[2][0] = mat.a3;
		converted.columns[2][1] = mat.b3;
		converted.columns[2][2] = mat.c3;
		converted.columns[2][3] = mat.d3;

		converted.columns[3][0] = mat.a4;
		converted.columns[3][1] = mat.b4;
		converted.columns[3][2] = mat.c4;
		converted.columns[3][3] = mat.d4;
		return converted;
	}

	static Math::Vec3 ConvertAiVector(const aiVector3D& vec)
	{
		return Math::Vec3(vec.x, vec.y, vec.z);
	}

	static Math::Quat ConvertAiQuat(const aiQuaternion& quat)
	{
		return Math::Quat(quat.x, quat.y, quat.z, quat.w);
	}
}
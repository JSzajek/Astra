#pragma once

#include <assimp/anim.h>

#include "Astra/math/Mat4.h"
#include "Astra/math/Vec3.h"
#include "Astra/math/Quat.h"

namespace Astra::Graphics
{
	static Math::Mat4 ConvertAiMatrix(const aiMatrix4x4& mat)
	{
		return Math::Mat4(mat.a1, mat.b1, mat.c1, mat.d1,
						  mat.a2, mat.b2, mat.c2, mat.d2, 
						  mat.a3, mat.b3, mat.c3, mat.d3, 
						  mat.a4, mat.b4, mat.c4, mat.d4);
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
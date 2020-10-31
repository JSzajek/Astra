#pragma once

#include "../../math/Maths.h"

namespace Astra::Graphics
{
	struct Spatial
	{
		union
		{
			float data[3 * 3];
			Math::Vec3 rows[3];
		};

		Spatial()
		{
			for (int i = 0; i < 3 * 3; i++)
			{
				data[i] = i < 6 ? 0 : 1;
			}
		}

		Spatial(const Spatial& other)
		{
			memcpy(data, other.data, 3 * 3 * sizeof(float));
		}

		Spatial(const Math::Vec3& translation)
			: Spatial()
		{
			data[0] = translation.x;
			data[1] = translation.y;
			data[2] = translation.z;
		}

		Spatial(const Math::Vec3& translation, const Math::Vec3& rotation, const Math::Vec3& scale)
			: Spatial()
		{
			data[0] = translation.x;
			data[1] = translation.y;
			data[2] = translation.z;

			data[3] = rotation.x;
			data[4] = rotation.y;
			data[5] = rotation.z;

			data[6] = scale.x;
			data[7] = scale.y;
			data[8] = scale.z;
		}

		float& operator()(const unsigned int& row, const unsigned int& column)
		{
			return data[row + column * 3];
		}
		
		Math::Vec3& operator[](const unsigned int& index)
		{
			return rows[index];
		}
		
		virtual inline const Math::Vec3& GetTranslation() const { return rows[0]; }
		virtual inline const Math::Vec3& GetRotation() const { return rows[1]; }
		virtual inline const Math::Vec3& GetScale() const { return rows[2]; }

		virtual inline Math::Vec3& Translation() { return rows[0]; }
		virtual inline Math::Vec3& Rotation() { return rows[1]; }
		virtual inline Math::Vec3& Scale() { return rows[2]; }

		virtual inline void SetTranslation(const Math::Vec3& translation) { rows[0] = translation; }
	};
}
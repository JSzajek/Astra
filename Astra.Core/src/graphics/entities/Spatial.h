#pragma once

#include "../../math/Maths.h"

namespace Astra::Graphics
{
	#define TRANSLATION 0
	#define ROTATION 1
	#define SCALE 2

	#define SUM_EQ 0
	#define EQ 1
	#define SUB_EQ 2

	#define X 0
	#define Y 1
	#define Z 2

	struct Spatial
	{
	private:
		Math::Mat4* modelMatrix;
		Math::Mat4* normalMatrix;
	public:
		union
		{
			float data[3 * 3];
			Math::Vec3 rows[3];
		};

		Spatial();
		Spatial(const Spatial& other);
		Spatial(const Math::Vec3& translation);
		Spatial(const Math::Vec3& translation, const Math::Vec3& rotation, const Math::Vec3& scale);
		~Spatial();

		float& operator()(const unsigned int& row, const unsigned int& column);
		
		Math::Vec3& operator[](const unsigned int& index);
		
		virtual inline const Math::Vec3& GetTranslation() const { return rows[0]; }
		virtual inline const Math::Vec3& GetRotation() const { return rows[1]; }
		virtual inline const Math::Vec3& GetScale() const { return rows[2]; }

		virtual inline Math::Vec3* Translation() { return &rows[0]; }
		virtual inline Math::Vec3* Rotation() { return &rows[1]; }
		virtual inline Math::Vec3* Scale() { return &rows[2]; }

		virtual inline void SetTranslation(const Math::Vec3& translation) { rows[0] = translation; }

		inline const Math::Mat4* const GetModelMatrix() const { return modelMatrix; }
		inline const Math::Mat4* const GetNormalMatrix() const { return normalMatrix; }

		virtual void operator()(unsigned int _type, unsigned int _op, unsigned int _index, float _val);
	protected:
		virtual void UpdateMatrices();
	private:
		void UpdateVector(Math::Vec3* _vec, unsigned int _op, unsigned int _index, float _val);
	};
}
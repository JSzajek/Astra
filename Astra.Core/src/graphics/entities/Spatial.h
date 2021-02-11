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

	#define X_POS 0
	#define Y_POS 1
	#define Z_POS 2

	struct Spatial
	{
	private:
		Math::Mat4* m_modelMatrix;
		Math::Mat4* m_normalMatrix;
	protected:
		union // TODO: Look into moving into private encapsulation
		{
			float m_data[3 * 3];
			Math::Vec3 m_rows[3];
		};
	public:
		Spatial();
		Spatial(const Spatial& other);
		Spatial(const Math::Vec3& translation);
		Spatial(const Math::Vec3& translation, const Math::Vec3& rotation, const Math::Vec3& scale);
		~Spatial();
		
		virtual inline const Math::Vec3& GetTranslation() const { return m_rows[0]; }
		virtual inline const Math::Vec3& GetRotation() const { return m_rows[1]; }
		virtual inline const Math::Vec3& GetScale() const { return m_rows[2]; }

		inline const Math::Mat4* const GetModelMatrix() const { return m_modelMatrix; }
		inline const Math::Mat4* const GetNormalMatrix() const { return m_normalMatrix; }

		virtual void SetTranslation(const Math::Vec3& translation);
		virtual void operator()(unsigned int _type, unsigned int _op, unsigned int _index, float _val);
	protected:
		float& operator()(const unsigned int& row, const unsigned int& column);
		Math::Vec3& operator[](const unsigned int& index);
		virtual void UpdateMatrices();
	private:
		void UpdateVector(Math::Vec3* _vec, unsigned int _op, unsigned int _index, float _val);
	};
}
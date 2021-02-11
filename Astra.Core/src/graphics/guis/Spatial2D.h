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

	struct Spatial2D
	{
	private:
		Math::Mat4* m_modelMatrix;
	protected:
		union // TODO: Look into moving into private encapsulation
		{
			float m_data[2 * 2];
			Math::Vec2 m_rows[2];
		};
		float m_rotation;
	public:
		Spatial2D();
		Spatial2D(const Spatial2D& other);
		Spatial2D(const Math::Vec2& translation);
		Spatial2D(const Math::Vec2& translation, float rotation, const Math::Vec2& scale);
		~Spatial2D();

		virtual inline const Math::Vec2& GetTranslation() const { return m_rows[0]; }
		virtual inline const float GetRotation() const { return m_rotation; }
		virtual inline const Math::Vec2& GetScale() const { return m_rows[1]; }

		inline const Math::Mat4* const GetModelMatrix() const { return m_modelMatrix; }

		virtual void SetTranslation(const Math::Vec2& translation);
		virtual void operator()(unsigned int _type, unsigned int _op, unsigned int _index, float _val);
		virtual void operator()(unsigned int _type, unsigned int _op, float _val);
	protected:
		float& operator()(const unsigned int& row, const unsigned int& column);
		Math::Vec2& operator[](const unsigned int& index);
		virtual void UpdateMatrices();
	private:
		void UpdateVector(Math::Vec2* _vec, unsigned int _op, unsigned int _index, float _val);
		void UpdateValue(float* _base, unsigned int _op, float _val);
	};

}
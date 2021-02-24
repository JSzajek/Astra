#include "astra_pch.h"

#include "Spatial2D.h"
#include "../../math/Mat4Utils.h"

namespace Astra::Graphics
{
	Spatial2D::Spatial2D(const char* const name)
		: LayerEntity(name), m_modelMatrix(new Math::Mat4(0)), m_rotation(0)
	{
		for (size_t i = 0; i < 2 * 2; i++)
		{
			m_data[i] = i < 2 ? 0.0f : 1.0f;
		}
		UpdateMatrices();
	}

	Spatial2D::Spatial2D(const Spatial2D& other)
		: m_modelMatrix(other.m_modelMatrix), m_rotation(other.m_rotation)
	{
		memcpy(m_data, other.m_data, sizeof(m_data));
	}

	Spatial2D::Spatial2D(const Math::Vec2& translation)
		: Spatial2D()
	{
		m_data[0] = translation.x;
		m_data[1] = translation.y;
		UpdateMatrices();
	}

	Spatial2D::Spatial2D(const char* const name, const Math::Vec2& translation)
		: Spatial2D(name)
	{
		m_data[0] = translation.x;
		m_data[1] = translation.y;
		UpdateMatrices();
	}

	Spatial2D::Spatial2D(const Math::Vec2& translation, float rotation, const Math::Vec2& scale)
		: Spatial2D()
	{
		m_data[0] = translation.x;
		m_data[1] = translation.y;

		m_rotation = rotation;

		m_data[2] = scale.x;
		m_data[3] = scale.y;
		UpdateMatrices();
	}

	Spatial2D::Spatial2D(const char* const name, const Math::Vec2& translation, float rotation, const Math::Vec2& scale)
		: Spatial2D(name)
	{
		m_data[0] = translation.x;
		m_data[1] = translation.y;

		m_rotation = rotation;

		m_data[2] = scale.x;
		m_data[3] = scale.y;
		UpdateMatrices();
	}

	Spatial2D::~Spatial2D()
	{
		//delete m_modelMatrix;
	}

	float& Spatial2D::operator()(const unsigned int& row, const unsigned int& column)
	{
		return m_data[column + row * 3];
	}

	Math::Vec2& Spatial2D::operator[](const unsigned int& index)
	{
		return m_rows[index];
	}

	void Spatial2D::SetTranslation(const Math::Vec2& translation)
	{
		m_rows[0] = translation;
		UpdateMatrices();
	}

	void Spatial2D::operator()(unsigned int _type, unsigned int _op, unsigned int _index, float _val)
	{
		switch (_type)
		{
		case TRANSLATION:
			UpdateVector(&m_rows[0], _op, _index, _val);
			break;
		case SCALE:
			UpdateVector(&m_rows[1], _op, _index, _val);
			break;
		default:
			return;
		}
		UpdateMatrices();
	}

	void Spatial2D::operator()(unsigned int _type, unsigned int _op, float _val)
	{
		if (_type != ROTATION)
		{
			ASTRA_CORE_WARN("Spatial2D: Only Rotation is Applicable For Single Value Operator.");
		}
		else
		{
			UpdateValue(&m_rotation, _op, _val);
			UpdateMatrices();
		}
	}
	
	void Spatial2D::UpdateMatrices()
	{
		*m_modelMatrix = Math::Mat4Utils::Transformation(this);
	}

	void Spatial2D::UpdateVector(Math::Vec2* _vec, unsigned int _op, unsigned int _index, float _val)
	{
		switch (_op)
		{
		case SUM_EQ:
			_vec->operator[](_index) += _val;
			break;
		case SUB_EQ:
			_vec->operator[](_index) -= _val;
			break;
		case EQ:
			_vec->operator[](_index) = _val;
			break;
		}
	}

	void Spatial2D::UpdateValue(float* _base, unsigned int _op, float _val)
	{
		switch (_op)
		{
		case SUM_EQ:
			*_base += _val;
			break;
		case SUB_EQ:
			*_base -= _val;
			break;
		case EQ:
			*_base = _val;
			break;
		}
	}
}
#include "astra_pch.h"

#include "Spatial.h"
#include "../../math/Mat4Utils.h"

namespace Astra::Graphics
{
	Spatial::Spatial()
		: m_modelMatrix(new Math::Mat4(0)), m_normalMatrix(new Math::Mat4(0))
	{
		for (size_t i = 0; i < 3 * 3; i++)
		{
			m_data[i] = i < 6 ? 0.0f : 1.0f;
		}
		UpdateMatrices();
	}

	Spatial::Spatial(const Spatial& other)
		: m_modelMatrix(other.m_modelMatrix), m_normalMatrix(other.m_normalMatrix)
	{
		memcpy(m_data, other.m_data, sizeof(m_data));
	}

	Spatial::Spatial(const Math::Vec3& translation)
		: Spatial()
	{
		m_data[0] = translation.x;
		m_data[1] = translation.y;
		m_data[2] = translation.z;
		UpdateMatrices();
	}

	Spatial::Spatial(const Math::Vec3& translation, const Math::Vec3& rotation, const Math::Vec3& scale)
		: Spatial()
	{
		m_data[0] = translation.x;
		m_data[1] = translation.y;
		m_data[2] = translation.z;

		m_data[3] = rotation.x;
		m_data[4] = rotation.y;
		m_data[5] = rotation.z;

		m_data[6] = scale.x;
		m_data[7] = scale.y;
		m_data[8] = scale.z;
		UpdateMatrices();
	}

	Spatial::~Spatial()
	{
		delete m_modelMatrix;
		delete m_normalMatrix;
	}

	float& Spatial::operator()(const unsigned int& row, const unsigned int& column)
	{
		return m_data[column + row * 3];
	}

	Math::Vec3& Spatial::operator[](const unsigned int& index)
	{
		return m_rows[index];
	}

	void Spatial::SetTranslation(const Math::Vec3& translation) 
	{
		m_rows[0] = translation;
		UpdateMatrices();
	}

	void Spatial::operator()(unsigned int _type, unsigned int _op, unsigned int _index, float _val)
	{
		switch (_type)
		{
		case TRANSLATION:
			UpdateVector(&m_rows[0], _op, _index, _val);
			break;
		case ROTATION:
			UpdateVector(&m_rows[1], _op, _index, _val);
			break;
		case SCALE:
			UpdateVector(&m_rows[2], _op, _index, _val);
			break;
		default:
			return;
		}
		UpdateMatrices();
	}

	void Spatial::UpdateMatrices()
	{
		*m_modelMatrix = Math::Mat4Utils::Transformation(this);
		*m_normalMatrix = m_modelMatrix->Inverse();
		m_normalMatrix->Transpose();
	}

	void Spatial::UpdateVector(Math::Vec3* _vec, unsigned int _op, unsigned int _index, float _val)
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
}
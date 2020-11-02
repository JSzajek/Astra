#include "Spatial.h"

#include "../../math/Mat4Utils.h"

namespace Astra::Graphics
{
	Spatial::Spatial()
		: modelMatrix(new Math::Mat4(0)), normalMatrix(new Math::Mat4(0))
	{
		for (int i = 0; i < 3 * 3; i++)
		{
			data[i] = i < 6 ? 0 : 1;
		}
		UpdateMatrices();
	}

	Spatial::Spatial(const Spatial& other)
		: modelMatrix(other.modelMatrix), normalMatrix(other.normalMatrix)
	{
		memcpy(data, other.data, 3 * 3 * sizeof(float));
	}

	Spatial::Spatial(const Math::Vec3& translation)
		: Spatial()
	{
		data[0] = translation.x;
		data[1] = translation.y;
		data[2] = translation.z;
		UpdateMatrices();
	}

	Spatial::Spatial(const Math::Vec3& translation, const Math::Vec3& rotation, const Math::Vec3& scale)
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
		UpdateMatrices();
	}

	Spatial::~Spatial()
	{
		delete modelMatrix;
		delete normalMatrix;
	}

	float& Spatial::operator()(const unsigned int& row, const unsigned int& column)
	{
		return data[row + column * 3];
	}

	Math::Vec3& Spatial::operator[](const unsigned int& index)
	{
		return rows[index];
	}

	void Spatial::operator()(unsigned int _type, unsigned int _op, unsigned int _index, float _val)
	{
		switch (_type)
		{
		case TRANSLATION:
			UpdateVector(Spatial::Translation(), _op, _index, _val);
			break;
		case ROTATION:
			UpdateVector(Spatial::Rotation(), _op, _index, _val);
			break;
		case SCALE:
			UpdateVector(Spatial::Scale(), _op, _index, _val);
			break;
		default:
			return;
		}
		UpdateMatrices();
	}

	void Spatial::UpdateMatrices()
	{
		*modelMatrix = Math::Mat4Utils::Transformation(this);
		*normalMatrix = modelMatrix->Inverse();
		normalMatrix->Transpose();
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
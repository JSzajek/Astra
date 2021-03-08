#pragma once

namespace Astra::Graphics
{
	class SymMatrix
	{
	private:
		double m_data[10]; // 4x4 upper triangle (4!)
	public:
		SymMatrix(double val = 0)
		{
			m_data[0] = val;
			m_data[1] = val;
			m_data[2] = val;
			m_data[3] = val;
			m_data[4] = val;
			m_data[5] = val;
			m_data[6] = val;
			m_data[7] = val;
			m_data[8] = val;
			m_data[9] = val;
		}

		SymMatrix(double m11, double m12, double m13, double m14,
							 double m22, double m23, double m24,
										double m33, double m34,
			double m44)
		{
			m_data[0] = m11;
			m_data[1] = m12;
			m_data[2] = m13;
			m_data[3] = m14;
			m_data[4] = m22;
			m_data[5] = m23;
			m_data[6] = m24;
			m_data[7] = m33;
			m_data[8] = m34;
			m_data[9] = m44;
		}

		SymMatrix(double a, double b, double c, double d)
		{
			m_data[0] = a * a;
			m_data[1] = a * b;
			m_data[2] = a * c;
			m_data[3] = a * d;
			m_data[4] = b * b;
			m_data[5] = b * c;
			m_data[6] = b * d;
			m_data[7] = c * c;
			m_data[8] = c * d;
			m_data[9] = d * d;
		}

		float operator[](int index) const { return m_data[index]; }

		double Determinant(int a11, int a12, int a13,
						  int a21, int a22, int a23,
						  int a31, int a32, int a33)
		{
			return m_data[a11] * m_data[a22] * m_data[a33] + m_data[a13] * m_data[a21] * m_data[a32] + m_data[a12] * m_data[a23] * m_data[a31]
					- m_data[a13] * m_data[a22] * m_data[a31] - m_data[a11] * m_data[a23] * m_data[a32] - m_data[a12] * m_data[a21] * m_data[a33];
		}

		const SymMatrix operator+(const SymMatrix r_val)
		{
			return SymMatrix(m_data[0] + r_val[0], 
							 m_data[1] + r_val[1],
							 m_data[2] + r_val[2],
						     m_data[3] + r_val[3], 
							 m_data[4] + r_val[4],
							 m_data[5] + r_val[5],
							 m_data[6] + r_val[6], 
							 m_data[7] + r_val[7],
							 m_data[8] + r_val[8],
							 m_data[9] + r_val[9]);
		}

		SymMatrix& operator+=(const SymMatrix& r_val)
		{
			m_data[0] += r_val[0];
			m_data[1] += r_val[1];
			m_data[2] += r_val[2];
			m_data[3] += r_val[3];
			m_data[4] += r_val[4];
			m_data[5] += r_val[5];
			m_data[6] += r_val[6];
			m_data[7] += r_val[7];
			m_data[8] += r_val[8];
			m_data[9] += r_val[9];
			return *this;

		}
	};
}
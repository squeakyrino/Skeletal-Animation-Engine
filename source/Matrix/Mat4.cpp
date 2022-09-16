#include <Matrix/Mat4.h>
#include <Exceptions/NotInversibleException.h>

namespace Engine {
	Mat4::Mat4() : m(4, std::vector<float>(4, 0))
	{
	}
	Mat4::Mat4(float value) : m(4, std::vector<float>(4, value))
	{
	}

	Mat4::Mat4(std::vector<std::vector<float>> elems) : m(elems)
	{
	}

	Mat4& Mat4::operator+=(const Mat4 u)
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				this->m[i][j] += u[i][j];
			}
		}

		return *this;
	}

	Mat4& Mat4::operator-=(const Mat4 u)
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				this->m[i][j] -= u[i][j];
			}
		}

		return *this;
	}

	Mat4& Mat4::operator*=(const Mat4 u)
	{
		Mat4 temp = Mat4();

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					temp[i][j] += this->m[i][k] * u[k][j];
				}
			}
		}

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m[i][j] = temp[i][j];
			}
		}

		return *this;
	}

	Mat4& Mat4::operator*=(const float scalar)
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				this->m[i][j] *= scalar;
			}
		}

		return *this;
	}

	Mat4& Mat4::operator/=(const float scalar)
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				this->m[i][j] /= scalar;
			}
		}

		return *this;
	}

	Mat4 Mat4::transpose() const
	{
		std::vector<std::vector<float>> temp(4, std::vector<float>(4, 0));

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				temp[j][i] = m[i][j];
			}
		}

		return Mat4(temp);
	}

	Mat4 Mat4::columnMajor()
	{
		return this->transpose();
	}

	Mat4 Mat4::inverse()
	{
		std::vector<float> m = transpose().flatten();

		float det, inv[16];
		
		 inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0)
		throw NotInversibleException();

    det = 1.0f / det;

	float invOut[16];
    
	for (int i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

	std::vector<std::vector<float>> result(4, std::vector<float>(4,0));
	result[0][0] = invOut[0];
	result[0][1] = invOut[4];
	result[0][2] = invOut[8];
	result[0][3] = invOut[12];

	result[1][0] = invOut[1];
	result[1][1] = invOut[5];
	result[1][2] = invOut[9];
	result[1][3] = invOut[13];

	result[2][0] = invOut[2];
	result[2][1] = invOut[6];
	result[2][2] = invOut[10];
	result[2][3] = invOut[14];

	result[3][0] = invOut[3];
	result[3][1] = invOut[7];
	result[3][2] = invOut[11];
	result[3][3] = invOut[15];

	return Mat4(result);
	}

	Mat4 Mat4::transposeInverse()
	{
		return inverse().transpose();
	}

	std::vector<float>& Mat4::operator[](std::size_t index)
	{
		return m[index];
	}

	const std::vector<float>& Mat4::operator[](std::size_t index) const
	{
		return m[index];
	}

	std::vector<float> Mat4::flatten() const
	{	
			std::vector<float> temp;

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					temp.push_back(this->m[i][j]);
				}
			}

			return temp;		
	}

	bool operator==(const Mat4& u, const Mat4& v)
	{

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (fabs(u[i][j] - v[i][j]) > EPSILON) {
					return false;
				}
			}
		}

		return true;
	}

	bool operator!=(const Mat4& u, const Mat4& v)
	{
		return !(u == v);
	}

	Mat4 operator+(Mat4 u, const Mat4& v)
	{
		return u += v;
	}

	Mat4 operator-(Mat4 u, const Mat4& v)
	{
		return u -= v;
	}

	Mat4 operator*(Mat4 u, const Mat4& v)
	{
		return u *= v;
	}

	Mat4 operator*(Mat4 u, const float scalar)
	{
		return u *= scalar;
	}

	Mat4 operator*(const float scalar, Mat4 u)
	{
		return u *= scalar;
	}

	Mat4 operator/(Mat4 u, const float scalar)
	{
		return u /= scalar;
	}

	Mat4 operator/(const float scalar, Mat4 u)
	{
		return u /= scalar;
	}

	Vec4 operator*(const Mat4 u, const Vec4 v)
	{
		return Vec4(u[0][0] * v.x + u[0][1] * v.y + u[0][2] * v.z + u[0][3] * v.w,
			u[1][0] * v.x + u[1][1] * v.y + u[1][2] * v.z + u[1][3] * v.w,
			u[2][0] * v.x + u[2][1] * v.y + u[2][2] * v.z + u[2][3] * v.w,
			u[3][0] * v.x + u[3][1] * v.y + u[3][2] * v.z + u[3][3] * v.w);
	}

	Vec4 operator*(const Vec4 v, const Mat4 u)
	{
		return u * v;
	}

	std::ostream& operator<<(std::ostream& os, const Mat4& obj)
	{
		for (int i = 0; i < 4; i++) {
			os << "|";
			os << obj[i][0] << "," << obj[i][1] << "," << obj[i][2] << "," << obj[i][3];
			os << "|" << std::endl;
		}

		return os;
	}
}
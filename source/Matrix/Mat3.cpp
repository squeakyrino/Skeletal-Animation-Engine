#include <Matrix/Mat3.h>
#include <Exceptions/NotInversibleException.h>

namespace Engine {
	Mat3::Mat3() : m(3, std::vector<float>(3, 0))
	{
	}
	Mat3::Mat3(float value) : m(3, std::vector<float>(3, value))
	{
	}

	Mat3::Mat3(float r1c1, float r1c2, float r1c3, float r2c1, float r2c2, float r2c3, float r3c1, float r3c2, float r3c3)
	{
		m = { {r1c1, r1c2, r1c3}, {r2c1, r2c2, r2c3}, {r3c1, r3c2, r3c3} };
	}

	Mat3::Mat3(std::vector<std::vector<float>> elems) : m(elems)
	{
	}

	Mat3& Mat3::operator+=(const Mat3 u)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				this->m[i][j] += u[i][j];
			}
		}

		return *this;
	}

	Mat3& Mat3::operator-=(const Mat3 u)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				this->m[i][j] -= u[i][j];
			}
		}

		return *this;
	}

	Mat3& Mat3::operator*=(const Mat3 u)
	{
		Mat3 temp = Mat3();

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					temp[i][j] += this->m[i][k] * u[k][j];
				}
			}
		}

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				m[i][j] = temp[i][j];
			}
		}

		return *this;
	}

	Mat3& Mat3::operator*=(const float scalar)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				this->m[i][j] *= scalar;
			}
		}

		return *this;
	}

	Mat3& Mat3::operator/=(const float scalar)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				this->m[i][j] /= scalar;
			}
		}

		return *this;
	}

	Mat3 Mat3::transpose()
	{
		std::vector<std::vector<float>> temp(3, std::vector<float>(3, 0));

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				temp[j][i] = m[i][j];
			}
		}

		return Mat3(temp);
	}

	float Mat3::determinant()
	{
		return (m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
			- m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
			+ m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));
	}

	Mat3 Mat3::inverse()
	{
		float det = this->determinant();

		if (fabs(det) < Engine::EPSILON) {
			throw NotInversibleException();
		}

		float reverseDet = 1 / det;

		std::vector<std::vector<float>> temp(3, std::vector<float>(3, 0));

		temp[0][0] = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * reverseDet;
		temp[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * reverseDet;
		temp[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * reverseDet;
		temp[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * reverseDet;
		temp[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * reverseDet;
		temp[1][2] = (m[1][0] * m[0][2] - m[0][0] * m[1][2]) * reverseDet;
		temp[2][0] = (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * reverseDet;
		temp[2][1] = (m[2][0] * m[0][1] - m[0][0] * m[2][1]) * reverseDet;
		temp[2][2] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * reverseDet;
	 											
		return Mat3(temp);
	}

	Mat3 Mat3::columnMajor()
	{
		return this->transpose();
	}

	std::vector<float>& Mat3::operator[](std::size_t index)
	{
		return m[index];
	}

	const std::vector<float>& Mat3::operator[](std::size_t index) const
	{
		return m[index];
	}

	std::vector<float> Mat3::flatten()
	{
		std::vector<float> temp;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				temp.push_back(this->m[i][j]);
			}
		}

		return temp;
	}

	bool operator==(const Mat3& u, const Mat3& v)
	{

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (fabs(u[i][j] - v[i][j]) > EPSILON) {
					return false;
				}
			}
		}

		return true;
	}

	bool operator!=(const Mat3& u, const Mat3& v)
	{
		return !(u == v);
	}

	Mat3 operator+(Mat3 u, const Mat3& v)
	{
		return u += v;
	}

	Mat3 operator-(Mat3 u, const Mat3& v)
	{
		return u -= v;
	}

	Mat3 operator*(Mat3 u, const Mat3& v)
	{
		return u *= v;
	}

	Mat3 operator*(Mat3 u, const float scalar)
	{
		return u *= scalar;
	}

	Mat3 operator*(const float scalar, Mat3 u)
	{
		return u *= scalar;
	}

	Mat3 operator/(Mat3 u, const float scalar)
	{
		return u /= scalar;
	}

	Mat3 operator/(const float scalar, Mat3 u)
	{
		return u /= scalar;
	}

	Vec3 operator*(const Mat3 u, const Vec3 v)
	{	
		return Vec3(u[0][0] * v.x + u[0][1] * v.y + u[0][2] * v.z,
					 u[1][0] * v.x + u[1][1] * v.y + u[1][2] * v.z,
					 u[2][0] * v.x + u[2][1] * v.y + u[2][2] * v.z);
	}

	Vec3 operator*(const Vec3 v, const Mat3 u)
	{
		return u * v;
	}

	std::ostream& operator<<(std::ostream& os, const Mat3& obj)
	{
		for (int i = 0; i < 3; i++) {
			os << "|";
			os << obj[i][0] << "," << obj[i][1] << "," << obj[i][2];
			os << "|" << std::endl;
		}

		return os;
	}
}
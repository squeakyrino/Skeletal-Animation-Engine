#include <Matrix/Mat2.h>
#include <EngineConstants.h>
#include <Exceptions/NotInversibleException.h>

#include <math.h>

namespace Engine {
	Mat2::Mat2() : m(2,std::vector<float>(2, 0))
	{
	}

	Mat2::Mat2(float value) : m(2, std::vector<float>(2, value))
	{
	}

	Mat2::Mat2(float r1c1, float r1c2, float r2c1, float r2c2) : m{ {r1c1, r1c2}, {r2c1, r2c2 } }
	{
	}

	Mat2::Mat2(std::vector<std::vector<float>> elems) : m(elems)
	{
	}

	Mat2& Mat2::operator+=(const Mat2 u)
	{
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				this->m[i][j] += u[i][j];
			}
		}

		return *this;
	}

	Mat2& Mat2::operator-=(const Mat2 u)
	{
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				this->m[i][j] -= u[i][j];
			}
		}

		return *this;
	}

	Mat2& Mat2::operator*=(const Mat2 u)
	{
		this->m[0][0] = this->m[0][0] * u[0][0] + this->m[0][1] * u[1][0];
		this->m[0][1] = this->m[0][0] * u[0][1] + this->m[0][1] * u[1][1];
		this->m[1][0] = this->m[1][0] * u[0][0] + this->m[1][1] * u[1][0];
		this->m[1][1] = this->m[1][0] * u[1][0] + this->m[1][1] * u[1][1];

		return *this;
	}

	Mat2& Mat2::operator*=(const float scalar)
	{
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				this->m[i][j] *= scalar;
			}
		}
		
		return *this;
	}

	Mat2& Mat2::operator/=(const float scalar)
	{
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				this->m[i][j] /= scalar;
			}
		}

		return *this;
	}

	Mat2 Mat2::transpose()
	{
		return Mat2(this->m[0][0], this->m[1][0], this->m[0][1], this->m[1][1]);
	}

	float Mat2::determinant()
	{
		return this->m[0][0] * this->m[1][1] - this->m[0][1] * this->m[1][0];
	}

	Mat2 Mat2::inverse()
	{
		float det = this->determinant();

		if (fabs(det) < EPSILON) {
			throw NotInversibleException();
		}

		float reverseDet = 1 / det;

		return Mat2(reverseDet * this->m[1][1], reverseDet * -this->m[0][1],
					reverseDet * -this->m[1][0], reverseDet * this->m[0][0]);
	}

	Mat2 Mat2::columnMajor()
	{
		return this->transpose();
	}

	std::vector<float>& Mat2::operator[](std::size_t index)
	{
		return m[index];
	}

	const std::vector<float>& Mat2::operator[](std::size_t index) const
	{
		return m[index];
	}

	std::vector<float> Mat2::flatten()
	{
		std::vector<float> temp;

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				temp.push_back(this->m[i][j]);
			}
		}

		return temp;
	}

	bool operator==(const Mat2& u, const Mat2& v)
	{

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				if (fabs(u[i][j] - v[i][j]) > EPSILON) {
					return false;
				}
			}
		}

		return true;
	}

	bool operator!=(const Mat2& u, const Mat2& v)
	{
		return !(u == v);
	}

	Mat2 operator+(Mat2 u, const Mat2& v)
	{
		return u += v;
	}

	Mat2 operator-(Mat2 u, const Mat2& v)
	{
		return u -= v;
	}

	Mat2 operator*(Mat2 u, const Mat2& v)
	{
		return u *= v;
	}

	Mat2 operator*(Mat2 u, const float scalar)
	{
		return u *= scalar;
	}

	Mat2 operator*(const float scalar, Mat2 u)
	{
		return u *= scalar;
	}

	Mat2 operator/(Mat2 u, const float scalar)
	{
		return u /= scalar;
	}

	Mat2 operator/(const float scalar, Mat2 u)
	{
		return u /= scalar;
	}

	Vec2 operator*(const Mat2 u, const Vec2 v)
	{
		return Vec2(u[0][0] * v.x + u[0][1] * v.y, u[1][0] * v.x + u[1][1] * v.y);
	}

	Vec2 operator*(const Vec2 v, const Mat2 u)
	{
		return u * v;
	}

	std::ostream& operator<<(std::ostream& os, const Mat2& obj)
	{
		for (int i = 0; i < 2; i++) {
			os << "|";
			os << obj[i][0] << "," << obj[i][1];
			os << "|" << std::endl;
		}

		return os;
	}
}

#pragma once
#include <EngineConstants.h>
#include <Vectors/Vec3.h>

#include <ostream>
#include <vector>

namespace Engine {
	class Mat3
	{
		std::vector<std::vector<float>> m;
	public:
		Mat3();
		Mat3(float value);
		Mat3(float r1c1, float r1c2, float r1c3, 
			float r2c1, float r2c2, float r2c3,
			float r3c1, float r3c2, float r3c3);
		Mat3(std::vector<std::vector<float>> elems);

		Mat3& operator+=(const Mat3 u);
		Mat3& operator-=(const Mat3 u);
		Mat3& operator*=(const Mat3 u);
		Mat3& operator*=(const float scalar);
		Mat3& operator/=(const float scalar);
		Mat3 transpose();
		float determinant();


		//Throws NotInversibleException if the matrix is not invertible
		Mat3 inverse();
		Mat3 columnMajor();

		std::vector<float>& operator[](std::size_t index);
		const std::vector<float>& operator[](std::size_t index) const;

		std::vector<float> flatten();
	};

	bool operator==(const Mat3& u, const Mat3& v);
	bool operator!=(const Mat3& u, const Mat3& v);

	Mat3 operator+(Mat3 u, const Mat3& v);
	Mat3 operator-(Mat3 u, const Mat3& v);

	Mat3 operator*(Mat3 u, const Mat3& v);

	Mat3 operator*(Mat3 u, const float scalar);
	Mat3 operator*(const float scalar, Mat3 u);

	Mat3 operator/(Mat3 u, const float scalar);
	Mat3 operator/(const float scalar, Mat3 u);

	Vec3 operator*(const Mat3 u, const Vec3 vec);
	Vec3 operator*(const Vec3 vec, const Mat3 u);

	std::ostream& operator<<(std::ostream& os, const Mat3& obj);
}

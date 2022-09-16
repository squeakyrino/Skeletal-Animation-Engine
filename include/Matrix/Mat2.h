#pragma once
#include <Vectors/Vec2.h>

#include <vector>
#include <ostream>

namespace Engine {
	class Mat2
	{
		std::vector<std::vector<float>> m;
	public:		
		Mat2();
		Mat2(float value);
		Mat2(float r1c1, float r1c2, float r2c1, float r2c2);
		Mat2(std::vector<std::vector<float>> elems);

		Mat2& operator+=(const Mat2 u);
		Mat2& operator-=(const Mat2 u);
		Mat2& operator*=(const Mat2 u);
		Mat2& operator*=(const float scalar);
		Mat2& operator/=(const float scalar);
		Mat2 transpose();
		float determinant();

		
		//Throws NotInversibleException if the matrix is not invertible		
		Mat2 inverse();
		Mat2 columnMajor();

		std::vector<float>& operator[](std::size_t index);
		const std::vector<float>& operator[](std::size_t index) const;

		std::vector<float> flatten();
	};

	bool operator==(const Mat2& u, const Mat2& v);
	bool operator!=(const Mat2& u, const Mat2& v);

	Mat2 operator+(Mat2 u, const Mat2& v);
	Mat2 operator-(Mat2 u, const Mat2& v);

	Mat2 operator*(Mat2 u, const Mat2& v);

	Mat2 operator*(Mat2 u, const float scalar);
	Mat2 operator*(const float scalar, Mat2 u);

	Mat2 operator/(Mat2 u, const float scalar);
	Mat2 operator/(const float scalar, Mat2 u);

	Vec2 operator*(const Mat2 u, const Vec2 vec);
	Vec2 operator*(const Vec2 vec, const Mat2 u);

	std::ostream& operator<<(std::ostream& os, const Mat2& obj);
}

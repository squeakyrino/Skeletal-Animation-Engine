#pragma once
#include <EngineConstants.h>
#include <Vectors/Vec4.h>

#include <ostream>
#include <vector>

namespace Engine {
	class Mat4
	{
		std::vector<std::vector<float>> m;
	public:
		Mat4();
		Mat4(float value);
		Mat4(std::vector<std::vector<float>> elems);

		Mat4& operator+=(const Mat4 u);
		Mat4& operator-=(const Mat4 u);
		Mat4& operator*=(const Mat4 u);
		Mat4& operator*=(const float scalar);
		Mat4& operator/=(const float scalar);

		Mat4 transpose() const;
		Mat4 columnMajor();
		Mat4 inverse();
		Mat4 transposeInverse();

		std::vector<float>& operator[](std::size_t index);
		const std::vector<float>& operator[](std::size_t index) const;

		std::vector<float> flatten() const;
	};

	bool operator==(const Mat4& u, const Mat4& v);
	bool operator!=(const Mat4& u, const Mat4& v);

	Mat4 operator+(Mat4 u, const Mat4& v);
	Mat4 operator-(Mat4 u, const Mat4& v);

	Mat4 operator*(Mat4 u, const Mat4& v);

	Mat4 operator*(Mat4 u, const float scalar);
	Mat4 operator*(const float scalar, Mat4 u);

	Mat4 operator/(Mat4 u, const float scalar);
	Mat4 operator/(const float scalar, Mat4 u);

	Vec4 operator*(const Mat4 u, const Vec4 vec);
	Vec4 operator*(const Vec4 vec, const Mat4 u);

	std::ostream& operator<<(std::ostream& os, const Mat4& obj);
}
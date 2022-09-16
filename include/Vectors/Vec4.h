#pragma once

#include <ostream>
#include <Vectors/Vec3.h>

namespace Engine
{
	class Vec4 {

	public:

		float x, y, z, w;

		Vec4();
		Vec4(float x);
		Vec4(float x, float y, float z);
		Vec4(const Vec3& v);
		Vec4(float x, float y, float z, float w);
		Vec4(const Vec4& v);

		float dot(const Vec4 v);

		Vec4 normalize();

		Vec4& operator+=(const Vec4 v);
		Vec4& operator-=(const Vec4 v);

		Vec4& operator*=(const float x);
		Vec4& operator/=(const float x);

		friend Vec4 operator+(Vec4 v, const Vec4& w);
		friend Vec4 operator-(Vec4 v, const Vec4& w);

		friend Vec4 operator*(Vec4 v, const float x);
		friend Vec4 operator/(Vec4 v, const float x);


		bool operator==(const Vec4& w);
		bool operator!=(const Vec4& w);

		friend std::ostream& operator<<(std::ostream& os, const Vec4& v);

		Vec3 toVec3();

	};
}
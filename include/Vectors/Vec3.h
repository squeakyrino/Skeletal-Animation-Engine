#pragma once

#include <ostream>
#include <Vectors/Vec2.h>

namespace Engine 
{
	class Vec3 {

	public:

		float x, y, z;

		Vec3();
		Vec3(float x);
		Vec3(float x, float y);
		Vec3(const Vec2& v);
		Vec3(float x, float y, float z);
		Vec3(const Vec3& v);


		Vec3& operator+=(const Vec3 v);
		Vec3& operator-=(const Vec3 v);
		Vec3& operator*=(const float x);
		Vec3& operator/=(const float x);

		Vec3 operator-();

		friend Vec3 operator+(Vec3 v, const Vec3& w);

		friend Vec3 operator-(const Vec3& w, Vec3 v);

		friend Vec3 operator*(Vec3 v, const float x);
		friend Vec3 operator*(const float x, Vec3 v);

		friend Vec3 operator/(Vec3 v, const float x);
		friend Vec3 operator/(const float x, Vec3 v);

		float dot(const Vec3 v);
		Vec3 cross(const Vec3 v);

		Vec3 normalize();

		Vec3 rotate(Vec3& v, float angle);

		float sqrMagnitude();
		float magnitude();

		Vec3 round();

		friend std::ostream& operator<<(std::ostream& os, const Vec3& v);

		static Vec3 xAxis();
		static Vec3 yAxis();
		static Vec3 zAxis();

		//t - Has to be between 0-1
		static Vec3 interpolate(const Vec3& a, const Vec3& b, const float& t);
	};

	bool operator==(Vec3 v1, const Vec3& v2);
	bool operator!=(Vec3 v1, const Vec3& v2);
}
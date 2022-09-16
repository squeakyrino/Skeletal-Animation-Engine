#pragma once

#include <ostream>

namespace Engine
{
	class Vec2 {

	public:

		float x, y;

		Vec2();
		Vec2(float x);
		Vec2(float x, float y);

		Vec2& operator+=(const Vec2 v);
		Vec2& operator-=(const Vec2 v);

		Vec2& operator*=(const float x);
		Vec2& operator/=(const float x);

		friend Vec2 operator+(Vec2 v, const Vec2& w);
		friend Vec2 operator-(Vec2 v, const Vec2& w);

		friend Vec2 operator*(Vec2 v, const float x);
		friend Vec2 operator/(Vec2 v, const float x);

		bool operator==(const Vec2& w);
		bool operator!=(const Vec2& w);

		float dot(const Vec2 v);

		Vec2 normalize();

		float sqrMagnitude();
		float magnitude();

		friend std::ostream& operator<<(std::ostream& os, const Vec2& v);
	};
}
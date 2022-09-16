#include <Vectors/Vec2.h>
#include <EngineConstants.h>

namespace Engine
{
	Vec2::Vec2() : x(0), y(0) {};

	Vec2::Vec2(float a) : x(a), y(a) {};

	Vec2::Vec2(float a, float b) {
		this->x = a;
		this->y = b;
	};

	Vec2& Vec2::operator+=(const Vec2 v) {
		this->x += v.x;
		this->y += v.y;
		return *this;
	};

	Vec2& Vec2::operator-=(const Vec2 v) {
		this->x -= v.x;
		this->y -= v.y;
		return *this;
	};

	Vec2& Vec2::operator*=(const float x) {
		this->x *= x;
		this->y *= x;
		return *this;
	};

	Vec2& Vec2::operator/=(const float x) {
		this->x /= x;
		this->y /= x;
		return *this;
	};

	Vec2 operator+(Vec2 v, const Vec2& w) {
		Vec2 r;
		r.x = v.x + w.x;
		r.y = v.y + w.y;
		return r;
	};

	Vec2 operator-(Vec2 v, const Vec2& w) {
		Vec2 r;
		r.x = v.x - w.x;
		r.y = v.y - w.y;
		return r;
	};

	Vec2 operator*(Vec2 v, const float x) {
		Vec2 r;
		r.x = v.x * x;
		r.y = v.y * x;
		return r;
	};

	Vec2 operator/(Vec2 v, const float x) {
		Vec2 r;
		r.x = v.x / x;
		r.y = v.y / x;
		return r;
	};

	bool Vec2::operator==(const Vec2& w) {
		return std::abs(this->x - w.x) < EPSILON && std::abs(this->y - w.y) < EPSILON;
	};

	bool Vec2::operator!=(const Vec2& w) {
		return !(*this == w);
	};

	float Vec2::dot(const Vec2 v) {
		float a;
		a = (this->x * v.x) + (this->y * v.y);
		return a;
	};


	Vec2 Vec2::normalize() {
		float l = sqrt((pow(this->x, 2) + pow(this->y, 2)));
		this->x /= l;
		this->y /= l;
		return *this;
	};

	float Vec2::sqrMagnitude()
	{
		return pow(this->x, 2) + pow(this->y, 2);
	}

	float Vec2::magnitude()
	{
		return sqrt(sqrMagnitude());
	};

	std::ostream& operator<<(std::ostream& out, Vec2 const& v) {
		out << "(" << v.x << "," << v.y << ")";
		return out;
	};
}
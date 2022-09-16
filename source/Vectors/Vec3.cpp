#include <Vectors/Vec3.h>
#include <EngineConstants.h>

namespace Engine
{
	Vec3::Vec3() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	};

	Vec3::Vec3(float a) {
		this->x = a;
		this->y = a;
		this->z = a;
	};

	Vec3::Vec3(float a, float b) {
		this->x = a;
		this->y = b;
		this->z = 0;
	};

	Vec3::Vec3(const Vec2& v) {
		this->x = v.x;
		this->y = v.y;
		this->z = 0;
	};

	Vec3::Vec3(float a, float b, float c) {
		this->x = a;
		this->y = b;
		this->z = c;
	};

	Vec3::Vec3(const Vec3& v) {
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
	};

	Vec3& Vec3::operator+=(const Vec3 v) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		return *this;
	};

	Vec3& Vec3::operator-=(const Vec3 v) {
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		return *this;
	};

	Vec3& Vec3::operator*=(const float x) {
		this->x *= x;
		this->y *= x;
		this->z *= x;
		return *this;
	};

	Vec3& Vec3::operator/=(const float x) {
		this->x /= x;
		this->y /= x;
		this->z /= x;
		return *this;
	};

	Vec3 Vec3::operator-()
	{
		return Vec3(-x, -y, -z);
	};

	Vec3 operator+(Vec3 v, const Vec3& w) {
		Vec3 r;
		r.x = v.x + w.x;
		r.y = v.y + w.y;
		r.z = v.z + w.z;
		return r;
	};


	Vec3 operator-(const Vec3& w, Vec3 v) {
		Vec3 r;
		r.x = w.x - v.x;
		r.y = w.y - v.y;
		r.z = w.z - v.z;
		return r;
	};


	Vec3 operator*(Vec3 v, const float x) {
		Vec3 r;
		r.x = v.x * x;
		r.y = v.y * x;
		r.z = v.z * x;
		return r;
	};

	Vec3 operator*(const float x, Vec3 v) {
		Vec3 r;
		r.x = v.x * x;
		r.y = v.y * x;
		r.z = v.z * x;
		return r;
	};

	Vec3 operator/(Vec3 v, const float x) {
		Vec3 r;
		r.x = v.x / x;
		r.y = v.y / x;
		r.z = v.z / x;
		return r;
	};

	Vec3 operator/(const float x, Vec3 v) {
		Vec3 r;
		r.x = v.x / x;
		r.y = v.y / x;
		r.z = v.z / x;
		return r;
	};


	float Vec3::dot(const Vec3 v) {
		float a;
		a = (this->x * v.x) + (this->y * v.y) + (this->z * v.z);
		return a;
	};

	Vec3 Vec3::cross(const Vec3 v) {
		Vec3 r;
		r.x = (this->y * v.z) - (this->z * v.y);
		r.y = (this->z * v.x) - (this->x * v.z);
		r.z = (this->x * v.y) - (this->y * v.x);
		return r;
	};


	Vec3 Vec3::normalize() {

		Vec3 result;

		float lengthS = dot(*this);
		if (lengthS > 0.0001f * 0.0001f)
		{
			result = Vec3(x, y, z) * (1.0f / sqrtf(lengthS));
		}

		return result;
	};



	Vec3 Vec3::rotate(Vec3& v, float angle) {
		Vec3 r;
		Vec3 a = v.normalize();
		r = (cos(angle) * *this) + sin(angle) * (a.cross(*this)) + a * (a.dot(*this)) * (1 - cos(angle));
		return r.round();
	}

	float Vec3::sqrMagnitude()
	{
		return dot(*this);
	}

	float Vec3::magnitude()
	{
		return sqrt(sqrMagnitude());
	}

	Vec3 Vec3::round() {
		Vec3 r;
		if (std::abs(this->x) < EPSILON) r.x = 0;
		else r.x = this->x;
		if (std::abs(this->y) < EPSILON) r.y = 0;
		else r.y = this->y;
		if (std::abs(this->z) < EPSILON) r.z = 0;
		else r.z = this->z;
		return r;
	}

	Vec3 Vec3::xAxis()
	{
		return Vec3(1, 0, 0);
	}

	Vec3 Vec3::yAxis()
	{
		return Vec3(0, 1, 0);
	}

	Vec3 Vec3::zAxis()
	{
		return Vec3(0, 0, 1);
	}

	Vec3 Vec3::interpolate(const Vec3& a, const Vec3& b, const float& t)
	{
		return a * (1.0f - t) +  b * t;
	}

	std::ostream& operator<<(std::ostream& out, Vec3 const& v) {
		out << "(" << v.x << "," << v.y << "," << v.z << ")";
		return out;
	}

	bool operator==(Vec3 v1, const Vec3& v2)
	{
		return std::abs(v1.x - v2.x) < EPSILON && 
			std::abs(v1.y - v2.y) < EPSILON && 
			std::abs(v1.z - v2.z) < EPSILON;
	}

	bool operator!=(Vec3 v1, const Vec3& v2)
	{
		return !(v1 == v2);
	}
}
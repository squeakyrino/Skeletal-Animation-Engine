#include <Vectors/Vec4.h>
#include <EngineConstants.h>

namespace Engine
{
	Vec4::Vec4() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	};

	Vec4::Vec4(float a) {
		this->x = a;
		this->y = a;
		this->z = a;
		this->w = a;
	};

	Vec4::Vec4(float a, float b, float c) {
		this->x = a;
		this->y = b;
		this->z = c;
		this->w = 0;
	};

	Vec4::Vec4(const Vec3& v) {
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		this->w = 0;
	};

	Vec4::Vec4(float a, float b, float c, float d) {
		this->x = a;
		this->y = b;
		this->z = c;
		this->w = d;
	};

	Vec4::Vec4(const Vec4& v) {
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		this->w = v.w;
	}

	float Vec4::dot(const Vec4 v)
	{
		float a;
		a = (this->x * v.x) + (this->y * v.y) + (this->z * v.z) + (this->w * v.w);
		return a;
	}

	Vec4 Vec4::normalize()
	{
		Vec4 result;

		float lengthS = dot(*this);
		if (lengthS > 0.0001f * 0.0001f)
		{
			result = Vec4(x, y, z, w) * (1.0f / sqrtf(lengthS));
		}

		return result;
	};

	Vec4& Vec4::operator+=(const Vec4 v) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.x;
		this->w += v.y;
		return *this;
	};

	Vec4& Vec4::operator-=(const Vec4 v) {
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		this->w -= v.w;
		return *this;
	};

	Vec4& Vec4::operator*=(const float x) {
		this->x *= x;
		this->y *= x;
		this->z *= x;
		this->w *= x;
		return *this;
	};

	Vec4& Vec4::operator/=(const float x) {
		this->x /= x;
		this->y /= x;
		this->z /= x;
		this->w /= x;
		return *this;
	};

	Vec4 operator+(Vec4 v, const Vec4& u) {
		Vec4 r;
		r.x = v.x + u.x;
		r.y = v.y + u.y;
		r.z = v.x + u.x;
		r.w = v.y + u.y;
		return r;
	};

	Vec4 operator-(Vec4 v, const Vec4& u) {
		Vec4 r;
		r.x = v.x - u.x;
		r.y = v.y - u.y;
		r.z = v.x - u.x;
		r.w = v.y - u.y;
		return r;
	};

	Vec4 operator*(Vec4 v, const float x) {
		Vec4 r;
		r.x = v.x * x;
		r.y = v.y * x;
		r.z = v.z / x;
		r.w = v.w / x;
		return r;
	};

	Vec4 operator/(Vec4 v, const float x) {
		Vec4 r;
		r.x = v.x / x;
		r.y = v.y / x;
		r.z = v.z / x;
		r.w = v.w / x;
		return r;
	};


	bool Vec4::operator==(const Vec4& w) {

		return std::abs(this->x - w.x) < EPSILON && std::abs(this->y - w.y) < EPSILON
			&& std::abs(this->z - w.z) < EPSILON && std::abs(this->w - w.w) < EPSILON;
	};

	bool Vec4::operator!=(const Vec4& w) {
		return !(*this == w);
	};


	std::ostream& operator<<(std::ostream& out, Vec4 const& v) {
		out << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
		return out;
	};

	Vec3 Vec4::toVec3() {
		return Vec3(x, y, z);
	}
}
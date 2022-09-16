#include <Quaternion/Quaternion.h>
#include <EngineHelperFunctions.h>

namespace Engine
{
	Quaternion::Quaternion(float w, float x, float y, float z) : w(w), v(x, y, z)
	{
	}

	Quaternion::Quaternion() : w(1), v()
	{
	}

	Quaternion::Quaternion(float angle, Vec3 axis)
	{
		Vec3 axisN = axis.normalize();
		float angleRad = toRad(angle);
		
		float s = sin(angleRad / 2.0f);

		this->w = cos(angleRad / 2.0f);
		this->v = axisN * s;
	}

	float Quaternion::quadrance()
	{
		return w * w + v.sqrMagnitude();
	}

	float Quaternion::magnitude()
	{
		return sqrt(quadrance());
	}

	Quaternion Quaternion::normalize()
	{
		float s = 1 / magnitude();
		return Quaternion(w / s, v.x / s, v.y / s, v.z / s);
	}

	Quaternion Quaternion::conjugate()
	{
		Vec3 temp = -1 * v;
		return Quaternion(w, temp.x, temp.y, temp.z);
	}

	Quaternion Quaternion::inverse()
	{
		return conjugate() / quadrance();
	}

	Quaternion& Quaternion::operator*=(const float s)
	{
		w *= s;
		v *= s;

		return *this;
	}

	Quaternion& Quaternion::operator*=(const Quaternion q)
	{
		float wN = w * q.w - v.x * q.v.x - v.y * q.v.y - v.z * q.v.z;
		float iN = w * q.v.x + v.x * q.w + v.y * q.v.z - v.z * q.v.y;
		float jN = w * q.v.y + v.y * q.w + v.z * q.v.x - v.x * q.v.z;
		float kN = w * q.v.z + v.z * q.w + v.x * q.v.y - v.y * q.v.x;

		w = wN;
		v.x = iN;
		v.y = jN;
		v.z = kN;

		return *this;
	}

	Quaternion& Quaternion::operator/=(const float s)
	{
		this->w /= s;
		this->v /= s;

		return *this;
	}

	Quaternion& Quaternion::operator+=(const Quaternion q)
	{
		this->w += q.w;
		this->v += q.v;

		return *this;
	}

	Quaternion Quaternion::slerp(const Quaternion& q0, const Quaternion& q1, const float& t)
	{
		Quaternion z = q1;

		float cosTheta = q0.v.x * q1.v.x + q0.v.y * q1.v.y + q0.v.z * q1.v.z + q0.w * q1.w;

		if (cosTheta < 0)
		{
			z = -z;
			cosTheta = -cosTheta;
		}

		if (cosTheta > 1.0f - EPSILON)
		{
			//Revert to linear interpolation
			return lerp(q0, z, t);
		}
		else
		{
			float angle = acos(cosTheta);

			
			return (sinf((1 - t) * angle) * q0 + sinf(t * angle) * z) / sinf(angle);
		}
	}

	Quaternion Quaternion::lerp(const Quaternion& q0, const Quaternion& q1, const float& t)
	{		
		float cosTheta = q0.v.x * q1.v.x + q0.v.y * q1.v.y + q0.v.z * q1.v.z + q0.w * q1.w;

		Quaternion z = q0;

		if (cosTheta < 0)
			z = -z;

		return ((1 - t) * z + t * q1).normalize();
	}

	Quaternion operator*(Quaternion q1, const Quaternion q2)
	{
		return q1 *= q2;
	}

	Quaternion operator*(Quaternion q, const float s)
	{
		return q *= s;
	}

	Quaternion operator*(const float s, Quaternion q)
	{
		return q *= s;
	}

	Quaternion operator/(Quaternion q, const float s)
	{
		return q /= s;
	}

	Quaternion operator/(const float s, Quaternion q)
	{
		return q /= s;
	}

	Quaternion operator+(Quaternion q1, const Quaternion q2)
	{
		return q1 += q2;
	}

	Quaternion operator-(const Quaternion& q)
	{
		return -1 * q;
	}

	bool operator!=(const Quaternion& q1, const Quaternion& q2)
	{
		return !(q1 == q2);
	}

	bool operator==(const Quaternion& q1, const Quaternion& q2)
	{
		return std::abs(q1.w - q2.w) < EPSILON && q1.v == q2.v;
	}

	std::ostream& operator<<(std::ostream& os, const Quaternion& q)
	{
		os << "(" << q.w << ", " << q.v << ")";
		return os;
	}
}
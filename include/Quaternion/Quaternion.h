#pragma once
#include <Vectors\Vec3.h>

namespace Engine
{
	class Quaternion
	{
	public:
		//TODO: These are public for now. Modifying them directly isn't really something a user should do but idk
		//I wanted to move these into privates but I get problems with getters/functions needing friend and stuff
		float w;
		Vec3 v;

		Quaternion();
		Quaternion(float angle, Vec3 axis);
		Quaternion(float w, float x, float y, float z);

		float quadrance();
		float magnitude();

		Quaternion normalize();

		Quaternion conjugate();
		Quaternion inverse();

		Quaternion& operator*=(const float s);
		Quaternion& operator*=(const Quaternion q);

		Quaternion& operator/=(const float s);

		Quaternion& operator+=(const Quaternion q);


		static Quaternion slerp(const Quaternion& q0, const Quaternion& q1, const float& t);
		static Quaternion lerp(const Quaternion& q0, const Quaternion& q1, const float& t);
	};

	Quaternion operator*(Quaternion q1, const Quaternion q2);
	Quaternion operator*(Quaternion q, const float s);
	Quaternion operator*(const float s, Quaternion q);

	Quaternion operator/(Quaternion q, const float s);
	Quaternion operator/(const float s, Quaternion q);

	Quaternion operator+(Quaternion q1, const Quaternion q2);

	Quaternion operator-(const Quaternion& q);

	bool operator==(const Quaternion& q1, const Quaternion& q2);
	bool operator!=(const Quaternion& q1, const Quaternion& q2);
	

	std::ostream& operator<<(std::ostream& os, const Quaternion& q);
}
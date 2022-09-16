#pragma once
#include <Vectors\Vec3.h>
#include <Quaternion\Quaternion.h>
#include <Matrix\Mat4.h>

namespace Engine
{
	class JointTransform
	{
	private:
		//These are in bone-space
		Vec3 position;
		Quaternion rotation;
		Vec3 scale;

	public:
		JointTransform(const Vec3& position, const Quaternion& rotation, const Vec3& scale);

		Mat4 getLocalBoneTransform() const;

		Vec3 getLocalPos() const;
		Quaternion getLocalRot() const;
		Vec3 getLocalScale() const;

		//t - has to be between 0-1
		static JointTransform interpolate(const JointTransform& a, const JointTransform& b, const float& t);
	};

	bool operator==(const JointTransform& a, const JointTransform& b);
	bool operator!=(const JointTransform& a, const JointTransform& b);
}



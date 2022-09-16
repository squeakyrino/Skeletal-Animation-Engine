#include <Graphics/Animation/JointTransform.h>
#include <Matrix/MatrixFactory.h>

namespace Engine
{
    JointTransform::JointTransform(const Vec3& position, const Quaternion& rotation, const Vec3& scale) :
        position(position), rotation(rotation), scale(scale)
    {
    }

    Mat4 JointTransform::getLocalBoneTransform() const
    {
        return MatrixFactory::translationMatrix(position) * MatrixFactory::rotationByAxis(rotation) * MatrixFactory::scalingMatrix(scale);
    }

    Vec3 JointTransform::getLocalPos() const
    {
        return position;
    }

    Quaternion JointTransform::getLocalRot() const
    {
        return rotation;
    }

    Vec3 JointTransform::getLocalScale() const
    {
        return scale;
    }

    JointTransform JointTransform::interpolate(const JointTransform& a, const JointTransform& b, const float& t)
    {
        Vec3 intPos = Vec3::interpolate(a.position, b.position, t);
        Quaternion intRot = Quaternion::slerp(a.rotation, b.rotation, t);
        Vec3 intScale = Vec3::interpolate(a.scale, b.scale, t);
        
        return JointTransform(intPos, intRot, intScale);
    }

    bool operator==(const JointTransform& a, const JointTransform& b)
    {
        return a.getLocalPos() == b.getLocalPos() &&
            a.getLocalRot() == b.getLocalRot() &&
            a.getLocalScale() == b.getLocalScale();
    }

    bool operator!=(const JointTransform& a, const JointTransform& b)
    {
        return !(a == b);
    }
}
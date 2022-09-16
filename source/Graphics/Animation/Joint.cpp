#include <Graphics/Animation/Joint.h>

namespace Engine
{
	Joint::Joint(const int& id, const std::string& name, const Mat4& inverseBindTransform) :
		jointID(id), name(name), inverseBindTransform(inverseBindTransform)
	{
	}

	void Joint::addChild(Joint& child)
	{
		children.push_back(child);
	}

	Mat4 Joint::getAnimatedTransform()
	{
		return animatedModelTransform;
	}

	void Joint::setAnimatedTransform(const Mat4& transform)
	{
		animatedModelTransform = transform;
	}
	Mat4 Joint::getInverseBindTransform()
	{
		return inverseBindTransform;
	}
	int Joint::getIndex() const
	{
		return jointID;
	}

	const std::string Joint::getName()
	{
		return name;
	}

	std::vector<Joint>& Joint::getChildren()
	{
		return children;
	}
}
#include <Graphics/Animation/Animation.h>

namespace Engine
{
	Animation::Animation(float lengthSecs, const std::vector<JointAnimation> anims, const std::string& animationName) :
		length(lengthSecs), anims(anims), name(animationName)
	{
	}

	float Animation::getAnimLength()
	{
		return length;
	}

	std::vector<JointAnimation> Animation::getJointAnimations()
	{
		return anims;
	}

	std::string Animation::getName()
	{
		return name;
	}
}
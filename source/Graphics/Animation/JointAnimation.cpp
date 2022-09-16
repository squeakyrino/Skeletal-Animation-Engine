#include <Graphics/Animation/JointAnimation.h>

namespace Engine
{
	JointAnimation::JointAnimation(const std::vector<KeyFrame>& frames, const std::string& jointName) :
		frames(frames), jointName(jointName)
	{
	}

	std::vector<KeyFrame> JointAnimation::getKeyFrames()
	{
		return frames;
	}

	const std::string& JointAnimation::getName()
	{
		return jointName;
	}
}
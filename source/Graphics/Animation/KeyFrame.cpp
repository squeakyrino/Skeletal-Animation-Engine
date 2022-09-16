#include <Graphics\Animation\KeyFrame.h>

namespace Engine
{
	KeyFrame::KeyFrame(const float& timeStampInSeconds, const JointTransform& localBoneTransform) :
		timeStamp(timeStampInSeconds), localBoneTransform(localBoneTransform)
	{
	}

	float KeyFrame::getTimeStamp()
	{
		return timeStamp;
	}

	JointTransform KeyFrame::getLocalBoneTransform()
	{
		return localBoneTransform;
	}
}
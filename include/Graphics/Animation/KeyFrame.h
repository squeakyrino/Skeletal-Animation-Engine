#pragma once
#include <Graphics\Animation\JointTransform.h>

namespace Engine
{
	class KeyFrame
	{
	private:
		
		float timeStamp;
		//The bone transform at this specified keyframe
		JointTransform localBoneTransform;

	public:
		KeyFrame(const float& timeStampInSeconds, const JointTransform& localBoneTransform);

		float getTimeStamp();

		JointTransform getLocalBoneTransform();
	};
}
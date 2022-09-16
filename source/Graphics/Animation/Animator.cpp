#include <Graphics/Animation/Animator.h>
#include <Matrix/MatrixFactory.h>
#include <iostream>
#include <algorithm>
#include <cassert>

namespace Engine
{
	void Animator::increaseAnimTime(const float& elapsedTime)
	{
		animationTime += elapsedTime;
		
		if (animationTime > currentAnim.getAnimLength())
		{
			//Play once or loop depending on what was specified
			if (loop)
			{
				animationTime = fmod(animationTime, currentAnim.getAnimLength());
			}
			else
			{
				//Clamp the animation time
				animationTime = currentAnim.getAnimLength();
				stop = true;
			}
		}
	}

	std::map<std::string, Mat4> Animator::calculateCurrentAnimationPose()
	{
		std::map<std::string, Mat4> localBoneTransforms;
		//For every joint
		//NOTE: This order is fine for now
		for (auto& joint : currentAnim.getJointAnimations())
		{
			auto prevAndNext = getPrevAndNextKeyframe(joint);
			float progression = calculateProgression(prevAndNext.first, prevAndNext.second);

			Mat4 localBoneTransform = interpolateJointTransform(prevAndNext.first, prevAndNext.second, progression).getLocalBoneTransform();
			localBoneTransforms[joint.getName()] = localBoneTransform;
		}

		return localBoneTransforms;
	}

	std::pair<KeyFrame, KeyFrame> Animator::getPrevAndNextKeyframe(JointAnimation joint)
	{
		//Find the keyframe for this joint
		std::vector<KeyFrame> keyframes = joint.getKeyFrames();

		//TODO: Maybe keep track of what the current keyframes so we can avoid searching for them
		int previousFrameIndex = 0;

		bool foundKeyframe = false;

		for (int i = 0; i < keyframes.size() - 1; i++)
		{
			if (animationTime < keyframes[(size_t)i + 1].getTimeStamp())
			{
				previousFrameIndex = i;
				foundKeyframe = true;
				break;
			}
		}

		/*
		* Because the animation length can be bigger than the last keyframe's timestamp,
		* previousFrameIndex will not be changed because the animation time is after the last keyframe's timestamp
		* but before the animation length.
		* In this case, we return the last keyframe and the first keyframe of the animation in prev and next respectively
		* to interpolate between those 2 keyframes.
		*/

		if (!foundKeyframe)
		{
			return std::make_pair(keyframes[keyframes.size() - 1], keyframes[0]);
		}

		return std::make_pair(keyframes[previousFrameIndex], keyframes[previousFrameIndex + 1]);
	}

	float Animator::calculateProgression(KeyFrame prev, KeyFrame next)
	{
		float totalTime = next.getTimeStamp() - prev.getTimeStamp();
		float currTime = animationTime - prev.getTimeStamp();
		return currTime / totalTime;
	}

	JointTransform Animator::interpolateJointTransform(KeyFrame prev, KeyFrame next, float progression)
	{
		JointTransform prevTrans = prev.getLocalBoneTransform();
		JointTransform nextTrans = next.getLocalBoneTransform();

		if (progression < 0)	
		{
			/*
			* If progression is negative then it means our animation time went outside the last keyframe but is still within the
			* animation length period.
			* In this case we will interpolate between the last and first keyframe of the animation.
			* Because the the prev timestamp will be bigger than the next timestamp, calculateProgression will return a progression
			* in the negative range, [-1, 0] (with -1 being prev and 0 being next).
			* We must add 1 to transform that range back into [0, 1] so we know how close we are to next.
			*/
			progression += 1;			
		}	

		JointTransform interpolated = JointTransform::interpolate(prevTrans, nextTrans, progression);
		return interpolated;
	}

	void Animator::applyPoseToJoints(std::map<int,Mat4>& globalBoneTransforms, std::map<std::string, Mat4>& localBoneTransforms, Joint& joint, const Mat4& parentTransform)
	{
		Mat4 currentPoseLocalTransform = localBoneTransforms.find(joint.getName())->second;
		
		//Pose transform for the pose in model space
		Mat4 currentPoseTransform = parentTransform * currentPoseLocalTransform;

		for (auto& child : joint.getChildren())
		{
			applyPoseToJoints(globalBoneTransforms, localBoneTransforms, child, currentPoseTransform);
		}

		//Needs to be multiplied with the inverse bind transform
		//Basically you need to get the bone from the original bind position to the origin
		//And then apply the currentPoseTransform so the bone moves to the correct place
		currentPoseTransform = currentPoseTransform * joint.getInverseBindTransform();
		joint.setAnimatedTransform(currentPoseTransform);

		//Update the output with the final transform
		globalBoneTransforms[joint.getIndex()] = joint.getAnimatedTransform();
	}

	Animator::Animator(const Animation& animation, Joint rootJoint, bool loop) :
		currentAnim(animation), rootJoint(rootJoint), stop(false), animationTime(0),
		loop(loop)
	{
	}

	void Animator::update(const float& elapsedTime)
	{
		//If the animation is stopped, just return
		if (stop)
		{
			return;
		}
			
		increaseAnimTime(elapsedTime);
		std::map<std::string, Mat4> currPoseTransforms = calculateCurrentAnimationPose();
		applyPoseToJoints(animatedGlobalBoneTransforms, currPoseTransforms, rootJoint, MatrixFactory::identityMat4());
	}

	std::vector<Mat4> Animator::getAnimatedBoneTransforms()
	{
		std::vector<Mat4> result;
		result.reserve(animatedGlobalBoneTransforms.size());

		std::transform(animatedGlobalBoneTransforms.begin(),
						animatedGlobalBoneTransforms.end(),
						std::back_inserter(result),
						[](auto& pair) { return pair.second; });

		return result;
	}
}
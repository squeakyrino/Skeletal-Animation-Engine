#pragma once
#include <Graphics\Animation\Animation.h>
#include <Graphics\Animation\Joint.h>

#include <map>

namespace Engine
{
	class Animator
	{
	private:

		Animation currentAnim;
		float animationTime;
	
		bool loop;

		/* Contains the final animated transforms for each bone for a specified point in the animation.
		This is ordered by index which is the order sent to the GPU.*/
		std::map<int, Mat4> animatedGlobalBoneTransforms;
	public:
		//TEST:
		Joint rootJoint;

		bool stop;

	private:

		void increaseAnimTime(const float& elapsedTime);

		std::map<std::string, Mat4> calculateCurrentAnimationPose();

		//Get the keyframes of a given joint
		//Return the prev keyframe in first and next in second
		std::pair<KeyFrame, KeyFrame> getPrevAndNextKeyframe(JointAnimation joint);

		//Calculate the progression between 2 keyframes 
		float calculateProgression(KeyFrame prev, KeyFrame next);

		JointTransform interpolateJointTransform(KeyFrame prev, KeyFrame next, float progression);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="globalBoneTransforms">Return parameter. Returns the globalBoneTransforms.</param>
		/// <param name="localBoneTransforms"></param>
		/// <param name="rootJoint"></param>
		/// <param name="parentTransform"></param>
		void applyPoseToJoints(std::map<int, Mat4>& globalBoneTransforms, std::map<std::string, Mat4>& localBoneTransforms,
						Joint& rootJoint, const Mat4& parentTransform);

	public:
		Animator(const Animation& animation, Joint rootJoint, bool loop = true);

		void update(const float& elapsedTime);

		/// <summary>
		/// Returns the global animated bone transforms ready to sent to the GPU.
		/// </summary>
		/// <returns></returns>
		std::vector<Mat4> getAnimatedBoneTransforms();
	};
}



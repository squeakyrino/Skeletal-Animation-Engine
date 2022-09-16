#pragma once
#include <Graphics\Animation\JointAnimation.h>
#include <vector>

namespace Engine
{
	class Animation
	{
	private:
		
		//In seconds
		float length;
		//One JointAnimation for each joint in the model
		//This is organized by name which is fine for now tbh
		//Don't know if it should be organized in a better way
		std::vector<JointAnimation> anims;

		//Name of the animation
		std::string name;
	public:
		Animation(float lengthSecs, const std::vector<JointAnimation> keyframes, const std::string& animationName);

		float getAnimLength();

		std::vector<JointAnimation> getJointAnimations();

		std::string getName();
	};
}
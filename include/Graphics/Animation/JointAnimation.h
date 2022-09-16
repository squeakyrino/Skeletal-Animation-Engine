#pragma once
#include <Graphics\Animation\KeyFrame.h>
#include <vector>


namespace Engine
{
	class JointAnimation
	{
	private:
		//Each joint has it's list of keyframes
		//These are sorted by ASSIMP
		std::vector<KeyFrame> frames;

		std::string jointName;

	public:
		JointAnimation(const std::vector<KeyFrame>& frames, const std::string& jointName);

		std::vector<KeyFrame> getKeyFrames();

		const std::string& getName();
	};
}

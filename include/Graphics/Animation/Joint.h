#pragma once
#include <string>
#include <vector>
#include <Matrix\Mat4.h>

namespace Engine
{
	class Joint
	{
	private:
		int jointID;
		std::string name;
		std::vector<Joint> children;

		//This is transform in model matrix
		//The animator class will be responsible for getting this matrix
		Mat4 animatedModelTransform;

		Mat4 inverseBindTransform;

	public:
		Joint(const int& id, const std::string& name, const Mat4& inverseBindTransform);

		//TODO: check if it needs to be a copy or pointer
		//It does. Scuffed solution for now
		void addChild(Joint& child);

		Mat4 getAnimatedTransform();
		void setAnimatedTransform(const Mat4& transform);

		Mat4 getInverseBindTransform();

		int getIndex() const;

		const std::string getName();

		std::vector<Joint>& getChildren();
	};
}
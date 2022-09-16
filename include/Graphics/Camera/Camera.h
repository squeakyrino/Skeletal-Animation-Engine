#pragma once
#include <Vectors\Vec3.h>
#include <Matrix\Mat4.h>
#include <Engine Systems\InputSystem.h>

namespace Engine
{
	//TODO: This isn't really a camera but rather a FPS camera
	//Refactor this into another class eventually
	class Camera
	{
	public:
		Vec3 pos;
		Vec3 direction;
		Vec3 up;

		float pitch;
		float yaw;

		float lastX, lastY;

		bool isPerspective = true;
		Mat4 perspectiveMatrix;
		Mat4 orthoMatrix;
	public:

		Camera(float aspectRatio, float initialX, float initalY);

		void updateCamera(float elapsedTime, InputSystem* input);

		void toggleProjection();

		Mat4 getViewMat();
		Mat4 getProjMat();
	};
}
#include <Graphics/Camera/Camera.h>
#include <Matrix\MatrixFactory.h>

namespace Engine
{
	Camera::Camera(float aspectRatio, float initialX, float initalY) : pos(Vec3(0, 0, 0)), direction(0, 0, -1), up(0, 1, 0), yaw(-90.0f), pitch(0)
	{
		perspectiveMatrix = MatrixFactory::perspective(90.0f, aspectRatio, 0.001f, 100.0f);
		orthoMatrix = MatrixFactory::ortho(-1, 1, 1, -1, 0.1f, 100.0f);

		lastX = initialX;
		lastY = initalY;
	}

	void Camera::updateCamera(float elapsedTime, InputSystem* input)
	{

		float speed = 1;

		if (input->keyHeld(GLFW_KEY_LEFT_SHIFT))
			speed *= 10;

		if (input->keyHeld(GLFW_KEY_LEFT_ALT))
			speed /= 3;

		if (input->keyHeld(GLFW_KEY_W))
			pos = pos + elapsedTime * direction * speed;

		if (input->keyHeld(GLFW_KEY_S))
			pos -= elapsedTime * direction * speed;

		if (input->keyHeld(GLFW_KEY_A))
			pos -= direction.cross(up).normalize() * elapsedTime * speed;

		if (input->keyHeld(GLFW_KEY_D))
			pos += direction.cross(up).normalize() * elapsedTime * speed;

		if (input->keyHeld(GLFW_KEY_SPACE))
			pos += up * elapsedTime * speed;

		if (input->keyHeld(GLFW_KEY_LEFT_CONTROL))
			pos -= up * elapsedTime * speed;

		//Mouse
		Vec2 mousePos = input->queryMousePosition();

		float xOffset = mousePos.x - lastX;
		float yOffset = lastY - mousePos.y;
		lastX = mousePos.x;
		lastY = mousePos.y;

		float sens = 0.1f;
		xOffset *= sens;
		yOffset *= sens;

		yaw += xOffset;
		pitch += yOffset;

		//Clamp values
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		direction.x = cosf(pitch * 3.1415f / 180.0f) * cosf(yaw * 3.1415f / 180.0f);
		direction.y = sinf(pitch * 3.1415f / 180.0f);
		direction.z = cosf(pitch * 3.1415f / 180.0f) * sinf(yaw * 3.1415f / 180.0f);
		direction = direction.normalize();
	}

	void Camera::toggleProjection()
	{
		isPerspective = !isPerspective;
	}

	Mat4 Camera::getViewMat()
	{
		return MatrixFactory::viewMatrix(pos, pos + direction, up);
	}
	Mat4 Camera::getProjMat()
	{
		return isPerspective ? perspectiveMatrix : orthoMatrix;
	}
}
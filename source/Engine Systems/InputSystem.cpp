#include <Engine Systems/InputSystem.h>

namespace Engine
{
	InputSystem::InputSystem() { }

	void InputSystem::init(WindowSystem* ws)
	{
		keyMap = std::vector<KeyPress>(GLFW_KEY_LAST, KeyPress());
		mouseKeyMap = std::vector<KeyPress>(GLFW_MOUSE_BUTTON_LAST, KeyPress());
		windowHandle = ws->window;
	}

	void InputSystem::updateKeys()
	{
		//Update keyMap
		for(int i = 0; i < GLFW_KEY_LAST; i++)
		{		
			std::swap(keyMap[i].curr, keyMap[i].prev);			
			keyMap[i].curr = glfwGetKey(windowHandle, i);
		}
			
		//Update mouseKeyMap
		for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
		{
			std::swap(mouseKeyMap[i].curr, mouseKeyMap[i].prev);
			mouseKeyMap[i].curr = glfwGetMouseButton(windowHandle, i);
		}
	}

	bool InputSystem::keyPressed(int keycode)
	{
		//Get the key from the map
		//To be pressed it needs the curr state to be press but the prev state has to be release
		KeyPress temp = keyMap[keycode];
		int curr = temp.curr;
		int prev = temp.prev;

		if (curr != GLFW_PRESS)
			return false;
		else
		{
			if (prev == GLFW_RELEASE)
			{
				return true;
			}
		}

		return false;
	}

	bool InputSystem::keyHeld(int keycode)
	{
		//Get the key from the map
		//To be held it needs the curr state to be press and the prev state has to be press too
		KeyPress temp = keyMap[keycode];
		int curr = temp.curr;
		int prev = temp.prev;

		if (curr == GLFW_PRESS && prev == GLFW_PRESS)
			return true;
		else
			return false;
	}

	bool InputSystem::keyReleased(int keycode)
	{
		//Get the mouse button from the map
		//To be release the curr state needs to be release and the prev state has to be release
		KeyPress temp = keyMap[keycode];
		int curr = temp.curr;
		int prev = temp.prev;

		if (curr == GLFW_RELEASE && prev == GLFW_PRESS)
			return true;
		else
			return false;
	}

	Vec2 InputSystem::queryMousePosition()
	{
		double xPos, yPos;

		glfwGetCursorPos(windowHandle, &xPos, &yPos);
		
		return Vec2((float)xPos, (float)yPos);
	}

	bool InputSystem::mouseButtonPressed(int mouseKeycode)
	{
		//Get the mouse button from the map
		//To be pressed it needs the curr state to be press but the prev state has to be release
		KeyPress temp = mouseKeyMap[mouseKeycode];
		int curr = temp.curr;
		int prev = temp.prev;

		if (curr != GLFW_PRESS)
			return false;
		else
		{
			if (prev == GLFW_RELEASE)
			{
				return true;
			}
		}

		return false;
	}

	bool InputSystem::mouseButtonHeld(int mouseKeycode)
	{
		//Get the mouse button from the map
		//To be held it needs the curr state to be press and the prev state has to be press too
		KeyPress temp = mouseKeyMap[mouseKeycode];
		int curr = temp.curr;
		int prev = temp.prev;

		if (curr == GLFW_PRESS && prev == GLFW_PRESS)
			return true;
		else
			return false;
	}

	double InputSystem::getScrollWheel()
	{
		return scroll;
	}

	void InputSystem::resetScrollWheel()
	{
		scroll = 0;
	}
	
}


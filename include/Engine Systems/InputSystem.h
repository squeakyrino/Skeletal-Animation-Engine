#pragma once

#include <vector>
#include <Vectors\Vec2.h>
#include <Engine Systems\WindowSystem.h>

namespace Engine
{
	/*
	* Note: this class is poll only (except for the scroll wheel which is callback based) s.t. 
	* we can have a consistent input throughout a frame (i.e. a key state will be the same in all of the frame).
	* Scrollback approach can't do this because of a few problems with the scrollbacks.
	*/

	class InputSystem
	{
	private:

		friend class EngineApp;

		struct KeyPress
		{
			int prev, curr;
		};

		std::vector<KeyPress> keyMap;
		std::vector<KeyPress> mouseKeyMap;
		double scroll;		

		GLFWwindow* windowHandle;

		InputSystem();
		void updateKeys();

	public:
		
		void init(WindowSystem* ws);

		bool keyPressed(int keycode);

		bool keyHeld(int keycode);

		bool keyReleased(int keycode);

		Vec2 queryMousePosition();

		bool mouseButtonPressed(int mouseKeycode);

		bool mouseButtonHeld(int mouseKeycode);

		double getScrollWheel();

		//TODO: scrollwheel callback

		void resetScrollWheel();
		
#pragma region Deleted functions
		InputSystem(InputSystem const&) = delete;
		void operator=(InputSystem const&) = delete;
#pragma endregion

	};
}
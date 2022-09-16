#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>

#include <string>

namespace Engine
{
	class WindowSystem
	{
	private:
		friend class EngineApp;
		friend class InputSystem;
		GLFWwindow* window;

		int width;
		int height;
		
		WindowSystem();

		static void viewportCallback(GLFWwindow* window, int width, int height);

	public:
		
		void init(int width, int height, const std::string& windowName, bool rezisable);
		
		void handleEvents();

		void display();

		bool shouldClose();
		void close();

		int getWidth();
		int getHeight();	
	};
}
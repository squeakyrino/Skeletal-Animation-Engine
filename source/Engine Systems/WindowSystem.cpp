#include <Engine Systems/WindowSystem.h>
#include <stdexcept>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


namespace Engine
{
	WindowSystem::WindowSystem() { }

	void WindowSystem::viewportCallback(GLFWwindow* window, int width, int height)
	{		
		WindowSystem* windowPoint = (WindowSystem*)glfwGetWindowUserPointer(window);
		windowPoint->width = width;
		windowPoint->height = height;
		glViewport(0, 0, width, height);
	}

	void WindowSystem::init(int width, int height, const std::string& windowName, bool rezisable)
	{
		this->height = height;
		this->width = width;


		if (!glfwInit())
		{
			throw std::runtime_error("Failed to load GLFW.");
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

		glfwWindowHint(GLFW_RESIZABLE, rezisable ? GL_TRUE : GL_FALSE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		if (window == nullptr)
		{
			glfwTerminate();
			throw std::runtime_error("Failed to create GLFW window");
		}
		glfwMakeContextCurrent(window);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//Pointer magic. Basically assign the pointer of the WindowSystem object to this object so we can retrieve it later
		//In callbacks
		glfwSetWindowUserPointer(window, this);

		//Throw OpenGL init here because bruh
		glewExperimental = GL_TRUE;

		if (glewInit() != GLEW_OK)
		{
			close();
			throw std::runtime_error("Failed to create GLEW");
		}

		//Default opengl hints
		glViewport(0, 0, width, height);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
		glDepthRange(0.0, 1.0);
		glClearDepth(1.0);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);


		//Setup callbacks
		if (rezisable)
			//Setup window resize callback
			glfwSetFramebufferSizeCallback(window, viewportCallback);
	}

	void WindowSystem::handleEvents()
	{
		glfwPollEvents();
	}

	void WindowSystem::display()
	{
		glfwSwapBuffers(window);
	}

	bool WindowSystem::shouldClose()
	{
		return glfwWindowShouldClose(window);
	}

	void WindowSystem::close()
	{
		glfwTerminate();
	}

	int WindowSystem::getWidth()
	{
		return width;
	}

	int WindowSystem::getHeight()
	{
		return height;
	}

}

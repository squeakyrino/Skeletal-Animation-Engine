#include <EngineApp.h>

namespace Engine
{
	EngineApp::EngineApp(int width, int height, const std::string& windowName, bool rezisable)
	{

		windowSys = std::unique_ptr<WindowSystem>(new WindowSystem());
		windowSys->init(width, height, windowName, rezisable);
		
		inputSys = std::unique_ptr<InputSystem>(new InputSystem());
		inputSys->init(windowSys.get());
		
		/* Creates a camera at the origin looking at -Z.*/
		renderer = std::make_unique<Renderer>(width, height);
		Vec3 cameraInitPosition = inputSys->queryMousePosition();
		renderer->activeCamera = std::make_shared<Camera>((float)windowSys->getWidth() / windowSys->getHeight(), cameraInitPosition.x, cameraInitPosition.y);
	}

	const void EngineApp::start(int argc, char** argv)
	{
		if (!onCreate(argc, argv))
			return;

		bool shouldClose = false;

		glfwSetTime(0);

		auto tp1 = glfwGetTime();
		auto tp2 = glfwGetTime();

		while (!shouldClose && !windowSys->shouldClose())
		{
			//Handle timing
			tp2 = glfwGetTime();
			float elapsedTime = (float)(tp2 - tp1);
			tp1 = tp2;

			//Handle window events
			windowSys->handleEvents();

			//Update the input system
			inputSys->updateKeys();

			if (!update(elapsedTime))
				shouldClose = true;

			//TODO maybe use onDestroy?

			//This needs to be reset every frame as GLFW (What we depend on for input) doesn't do it itself
			//If we don't do this the engine will think we're infinitely scrolling
			inputSys->resetScrollWheel();

			windowSys->display();
		}
	}
}
#pragma once
#include <string>
#include <Engine Systems\WindowSystem.h>
#include <Engine Systems\InputSystem.h>
#include <Graphics\Renderer.h>

namespace Engine
{
	class EngineApp
	{	
	protected:
		//Systems
		std::unique_ptr<WindowSystem> windowSys;
		std::unique_ptr<InputSystem> inputSys;
		std::unique_ptr<Renderer> renderer;
		 
	public:

		EngineApp(int width, int height, const std::string& windowName, bool rezisable);

		const void start(int argc, char** argv);

		virtual bool onCreate(int argc, char** argv) = 0;

		virtual bool update(float elapsedTime) = 0;


	};
}
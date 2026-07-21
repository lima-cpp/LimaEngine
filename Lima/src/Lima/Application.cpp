#include "Application.h"
#include "Log.h"

namespace Lima {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (!LimaWindow.shouldClose())
		{
			glfwPollEvents();
		}
	}
}
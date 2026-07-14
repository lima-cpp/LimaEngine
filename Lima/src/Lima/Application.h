#pragma once

#include "Core.h"

namespace Lima {

	class LIMA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}


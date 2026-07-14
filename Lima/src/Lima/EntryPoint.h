#pragma once

#ifdef L_PLATFORM_WINDOWS

extern Lima::Application* Lima::CreateApplication();

int main(int argc, char** argv)
{
	Lima::Log::Init();
	L_CORE_WARN("Initialized Log!");
	L_INFO("Hello! This is Lima Engine!");

	auto app = Lima::CreateApplication();
	app->Run();
	delete app;
}


#endif
#pragma once

#ifdef L_PLATFORM_WINDOWS

#include <cstdlib>
#include <stdexcept>

int main() {	
	Lima::Log::Init();
	L_INFO("Log initialized");
	L_WARN("Lima Engine console");

	Lima::Application app{};

	try {
		app.Run();
	} catch (const std::exception &e) {
		L_CORE_ERROR("Application encountered an error: {0}", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


#endif
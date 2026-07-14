#pragma once

#ifdef L_PLATFORM_WINDOWS
	#ifdef LIMA_BUILD_DLL
		#define LIMA_API __declspec(dllexport)
	#else
		#define LIMA_API __declspec(dllimport)
	#endif
#else
	#error Lima only supports Windows!
#endif
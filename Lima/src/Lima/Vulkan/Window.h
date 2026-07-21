#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace Lima {

	class LimaWindow {
	public:
		LimaWindow(int w, int h, std::string name);
		~LimaWindow();

		LimaWindow(const LimaWindow&) = delete;
		LimaWindow &operator = (const LimaWindow&) = delete;
		
		bool shouldClose() { return glfwWindowShouldClose(window); }
		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

	private:
		void initWindow();

		const int width;
		const int height;

		std::string windowName;
		GLFWwindow* window;
	};
} // namespace Lima
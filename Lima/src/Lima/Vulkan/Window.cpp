#include "Window.h"

#include <stdexcept>

namespace Lima {

	LimaWindow::LimaWindow(int w, int h, std::string name) : width(w), height(h), windowName(name) {
		initWindow();
	}

	LimaWindow::~LimaWindow() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void LimaWindow::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}

	void LimaWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create window surface!");
		}
	}

	void LimaWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto m_LimaWindow = reinterpret_cast<LimaWindow*>(glfwGetWindowUserPointer(window));
		m_LimaWindow->framebufferResized = true;
		m_LimaWindow->width = width;
		m_LimaWindow->height = height;
	}

} // namespace Lima


#pragma once

#include "Vulkan/Window.h"
#include "Vulkan/Pipeline.h"
#include "Vulkan/LimaDevice.h"
#include "Vulkan/SwapChain.h"
#include "Core.h"

#include <memory>
#include <vector>

namespace Lima {

	class LIMA_API Application
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		Application();
		~Application();

		Application(const Application&) = delete;
		Application &operator = (const Application &) = delete;

		void Run();

	private:
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

		LimaWindow LimaWindow{ WIDTH, HEIGHT, "Lima Engine" };
		LimaDevice LimaDevice{ LimaWindow };
		LimaSwapChain LimaSwapChain{ LimaDevice, LimaWindow.getExtent() };
		std::unique_ptr<LimaPipeline> LimaPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
	};

	Application* CreateApplication();
} // namespace Lima


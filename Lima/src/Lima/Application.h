#pragma once

#include "Vulkan/Window.h"
#include "Vulkan/Pipeline.h"
#include "Vulkan/LimaDevice.h"
#include "Vulkan/SwapChain.h"
#include "Model.h"
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
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);

		LimaWindow m_LimaWindow{ WIDTH, HEIGHT, "Lima Engine" };
		LimaDevice m_LimaDevice{ m_LimaWindow };
		std::unique_ptr<LimaSwapChain> m_LimaSwapChain;
		std::unique_ptr<LimaPipeline> m_LimaPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::unique_ptr<LimaModel> m_LimaModel;
	};

	Application* CreateApplication();
} // namespace Lima


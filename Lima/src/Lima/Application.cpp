#include "Application.h"
#include "Log.h"

#include <stdexcept>

namespace Lima {

	Application::Application() {
		createPipelineLayout();
		createPipeline();
		createCommandBuffers();
	}

	Application::~Application() {
		vkDestroyPipelineLayout(LimaDevice.device(), pipelineLayout, nullptr);
	}

	void Application::Run()
	{
		while (!LimaWindow.shouldClose())
		{
			glfwPollEvents();
		}
	}

	void Application::createPipelineLayout() {
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(LimaDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create pipeline layout!");
		}
	}

	void Application::createPipeline() {
		auto pipelineConfig = LimaPipeline::defaultPipelineConfigInfo(LimaSwapChain.width(), LimaSwapChain.height());
		pipelineConfig.renderPass = LimaSwapChain.getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		LimaPipeline = std::make_unique<Lima::LimaPipeline>(
			LimaDevice, 
			"C:/LimaEngine/Lima/src/Lima/Shaders/simple_shader.vert.spv", 
			"C:/LimaEngine/Lima/src/Lima/Shaders/simple_shader.frag.spv", 
			pipelineConfig);
	}

	void Application::createCommandBuffers() {

	}

	void Application::drawFrame() {

	}

} // namespace Lima
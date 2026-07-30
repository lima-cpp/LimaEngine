#include "Application.h"
#include "Log.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>

namespace Lima {

	struct SimplePushConstantData {
		glm::mat2 transform{ 1.f };
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};

	Application::Application() {
		loadGameObjects();
		createPipelineLayout();
		recreateSwapChain();
		createCommandBuffers();
	}

	Application::~Application() {
		vkDestroyPipelineLayout(m_LimaDevice.device(), pipelineLayout, nullptr);
	}

	void Application::Run()
	{

		while (!m_LimaWindow.shouldClose()) {
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(m_LimaDevice.device());
	}

	void Application::loadGameObjects() {
		std::vector<LimaModel::Vertex> vertices{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};

		auto m_LimaModel = std::make_shared<LimaModel>(m_LimaDevice, vertices);

		std::vector<glm::vec3> colors{
			{1.f, .7f, .73f},
			{1.f, .87f, .73f},
			{1.f, 1.f, .73f},
			{.73f, 1.f, .8f},
			{.73, .88f, 1.f}
		};

		for (auto& color : colors) {
			color = glm::pow(color, glm::vec3{ 2.2f });
		}

		for (int i = 0; i < 40; i++) {
			auto triangle = LimaGameObject::createGameObject();
			triangle.model = m_LimaModel;
			triangle.transform2D.scale = glm::vec2(.5f) + i * 0.025f;
			triangle.transform2D.rotation = i * glm::pi<float>() * 0.025f;
			triangle.color = colors[i % colors.size()];
			m_GameObjects.push_back(std::move(triangle));
		}

		//auto triangle = LimaGameObject::createGameObject();
		//triangle.model = m_LimaModel;
		//triangle.color = { .1f, .8f, .1f };
		//triangle.transform2D.translation.x = .2f;
		//triangle.transform2D.scale = { 2.f, .5f };
		//triangle.transform2D.rotation = .25f * glm::two_pi<float>();

		//m_GameObjects.push_back(std::move(triangle));
	}

	void Application::createPipelineLayout() {

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(m_LimaDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create pipeline layout!");
		}
	}

	void Application::createPipeline() {
		assert(m_LimaSwapChain != nullptr && "Cannot create pipeline before swap chain!");
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout!");

		PipelineConfigInfo pipelineConfig{};
		LimaPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = m_LimaSwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		m_LimaPipeline = std::make_unique<Lima::LimaPipeline>(
			m_LimaDevice, 
			"C:/LimaEngine/Lima/src/Lima/Shaders/simple_shader.vert.spv",
			"C:/LimaEngine/Lima/src/Lima/Shaders/simple_shader.frag.spv",
			pipelineConfig);
	}

	void Application::recreateSwapChain() {
		auto extent = m_LimaWindow.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = m_LimaWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(m_LimaDevice.device());

		if (m_LimaSwapChain == nullptr) {
			m_LimaSwapChain = std::make_unique<LimaSwapChain>(m_LimaDevice, extent);
		}
		else {
			m_LimaSwapChain = std::make_unique<LimaSwapChain>(m_LimaDevice, extent, std::move(m_LimaSwapChain));
			if (m_LimaSwapChain->imageCount() != commandBuffers.size()) {
				freeCommandBuffers();
				createCommandBuffers();
			}
		}

		createPipeline();
	}

	void Application::createCommandBuffers() {
		commandBuffers.resize(m_LimaSwapChain->imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_LimaDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(m_LimaDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("Failed to allocate command buffers!");
		}
	}

	void Application::freeCommandBuffers() {
		vkFreeCommandBuffers(m_LimaDevice.device(), m_LimaDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();
	}

	void Application::recordCommandBuffer(int imageIndex) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("Failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_LimaSwapChain->getRenderPass();
		renderPassInfo.framebuffer = m_LimaSwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_LimaSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_LimaSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(m_LimaSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, m_LimaSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

		renderGameObjects(commandBuffers[imageIndex]);

		vkCmdEndRenderPass(commandBuffers[imageIndex]);
		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to record command buffer!");
		}
	}

	void Application::renderGameObjects(VkCommandBuffer commandBuffer) {
		int i = 0;

		m_LimaPipeline->bind(commandBuffer);

		for (auto& obj : m_GameObjects) {
			i += 1;
			obj.transform2D.rotation = glm::mod<float>(obj.transform2D.rotation + 0.001f * i, 2.f * glm::pi<float>());
			//obj.transform2D.rotation = glm::mod(obj.transform2D.rotation + 0.01f, glm::two_pi<float>());

			SimplePushConstantData push{};
			push.offset = obj.transform2D.translation;
			push.color = obj.color;
			push.transform = obj.transform2D.mat2();

			vkCmdPushConstants(commandBuffer, pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);

			obj.model->bind(commandBuffer);
			obj.model->draw(commandBuffer);
		}
	}

	void Application::drawFrame() {
		uint32_t imageIndex;
		auto result = m_LimaSwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("Failed to acquire swap chain image!");
		}

		recordCommandBuffer(imageIndex);
		result = m_LimaSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_LimaWindow.wasWindowResized()) {
			m_LimaWindow.resetWindowResizedFlag();
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to present swap chain image!");
		}
	}

} // namespace Lima
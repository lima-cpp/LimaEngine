#include "Renderer.h"
#include "../Log.h"

#include <stdexcept>
#include <array>

namespace Lima {

	LimaRenderer::LimaRenderer(LimaWindow& window, LimaDevice& device) : m_LimaWindow{ window }, m_LimaDevice{ device } {
		recreateSwapChain();
		createCommandBuffers();
	}

	LimaRenderer::~LimaRenderer() { freeCommandBuffers(); }

	void LimaRenderer::recreateSwapChain() {
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
			std::shared_ptr<LimaSwapChain> oldSwapChain = std::move(m_LimaSwapChain);
			m_LimaSwapChain = std::make_unique<LimaSwapChain>(m_LimaDevice, extent, oldSwapChain);

			if (!oldSwapChain->compareSwapFormats(*m_LimaSwapChain.get())) {
				throw std::runtime_error("Swap chain image or depth format has changed!");
			}

			if (m_LimaSwapChain->imageCount() != commandBuffers.size()) {
				freeCommandBuffers();
				createCommandBuffers();
			}
		}
	}

	void LimaRenderer::createCommandBuffers() {
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

	void LimaRenderer::freeCommandBuffers() {
		vkFreeCommandBuffers(m_LimaDevice.device(), m_LimaDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();
	}

	VkCommandBuffer LimaRenderer::beginFrame() {
		assert(!isFrameStarted && "Can't call beginFrame while already in progress!");
		
		auto result = m_LimaSwapChain->acquireNextImage(&currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("Failed to acquire swap chain image!");
		}

		isFrameStarted = true;

		auto commandBuffer = getCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("Failed to begin recording command buffer!");
		}

		return commandBuffer;
	}

	void LimaRenderer::endFrame() {
		assert(isFrameStarted && "Can't call endFrame while frame is not in progress!");
		auto commandBuffer = getCurrentCommandBuffer();

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("Failed to record command buffer!");
		}

		auto result = m_LimaSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_LimaWindow.wasWindowResized()) {
			m_LimaWindow.resetWindowResizedFlag();
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to present swap chain image!");
		}

		isFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % LimaSwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void LimaRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
		assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress!");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame!");


		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_LimaSwapChain->getRenderPass();
		renderPassInfo.framebuffer = m_LimaSwapChain->getFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_LimaSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_LimaSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(m_LimaSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, m_LimaSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void LimaRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
		assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress!");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame!");

		vkCmdEndRenderPass(commandBuffer);
	}

} // namespace Lima
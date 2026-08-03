#pragma once

#include "../Vulkan/Window.h"
#include "../Vulkan/LimaDevice.h"
#include "../Vulkan/SwapChain.h"

#include <memory>
#include <vector>
#include <cassert>

namespace Lima {

	class LimaRenderer
	{
	public:
		LimaRenderer(LimaWindow &window, LimaDevice &device);
		~LimaRenderer();

		LimaRenderer(const LimaRenderer&) = delete;
		LimaRenderer& operator = (const LimaRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return m_LimaSwapChain->getRenderPass(); }
		float getAspectRatio() const { return m_LimaSwapChain->extentAspectRatio(); }
		bool isFrameInProgress() const { return isFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(isFrameStarted && "Cannot get command buffer when frame is not in progress!");
			return commandBuffers[currentImageIndex];
		}

		int getFrameIndex() const {
			assert(isFrameStarted && "Cannot get frame index when frame is not in progress!");
			return currentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		LimaWindow& m_LimaWindow;
		LimaDevice& m_LimaDevice;
		std::unique_ptr<LimaSwapChain> m_LimaSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex = { 0 };
		bool isFrameStarted = { false };
	};
} // namespace Lima


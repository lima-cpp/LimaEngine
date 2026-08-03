#pragma once

#include "../Vulkan/Pipeline.h"
#include "../Vulkan/LimaDevice.h"
#include "../GameObject.h"
#include "Camera.h"

#include <memory>
#include <vector>

namespace Lima {

	class RenderSystem
	{
	public:
		RenderSystem(LimaDevice& device, VkRenderPass renderPass);
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator = (const RenderSystem&) = delete;

		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<LimaGameObject> &m_GameObjects, const LimaCamera &camera);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		LimaDevice& m_LimaDevice;

		std::unique_ptr<LimaPipeline> m_LimaPipeline;
		VkPipelineLayout pipelineLayout;
	};
} // namespace Lima


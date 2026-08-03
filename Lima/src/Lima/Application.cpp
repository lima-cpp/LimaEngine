#include "Application.h"
#include "Renderer/Camera.h"
#include "Renderer/RenderSystem.h"
#include "Systems/Movement.h"
#include "Log.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <cassert>
#include <chrono>
#include <stdexcept>
#include <array>

namespace Lima {

	Application::Application() {
		loadGameObjects();
	}

	Application::~Application() {
		
	}

	void Application::Run()
	{
		RenderSystem m_RenderSystem{ m_LimaDevice, m_LimaRenderer.getSwapChainRenderPass() };
		LimaCamera camera{};

		camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f , 2.5f));

		auto viewerObject = LimaGameObject::createGameObject();
		MovementSystem cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!m_LimaWindow.shouldClose()) {
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			frameTime = glm::min(frameTime, 0.1f);

			cameraController.moveInPlaneXZ(m_LimaWindow.getGLFWwindow(), frameTime, viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			float aspect = m_LimaRenderer.getAspectRatio();
			camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);
			
			if (auto commandBuffer = m_LimaRenderer.beginFrame()) {
				m_LimaRenderer.beginSwapChainRenderPass(commandBuffer);
				m_RenderSystem.renderGameObjects(commandBuffer, m_GameObjects, camera);
				m_LimaRenderer.endSwapChainRenderPass(commandBuffer);
				m_LimaRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(m_LimaDevice.device());
	}

	std::unique_ptr<LimaModel> createCubeModel(LimaDevice& device, glm::vec3 offset) {
		std::vector<LimaModel::Vertex> vertices {

			// left face (white)
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

			// right face (yellow)
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},

			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

			// bottom face (red)
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},

			// nose face (blue)
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

			// tail face (green)
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

		};
		for (auto& v : vertices) {
			v.position += offset;
		}
		return std::make_unique<LimaModel>(device, vertices);
	}

	void Application::loadGameObjects() {
		std::shared_ptr<LimaModel> m_LimaModel = createCubeModel(m_LimaDevice, { .0f, .0f, .0f });

		auto cube = LimaGameObject::createGameObject();
		cube.model = m_LimaModel;
		cube.transform.translation = { .0f, .0f, 2.5f };
		cube.transform.scale = { .5f, .5f, .5f };
		m_GameObjects.push_back(std::move(cube));
	}
} // namespace Lima
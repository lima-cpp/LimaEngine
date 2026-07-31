#pragma once

#include "Vulkan/Window.h"
#include "Vulkan/LimaDevice.h"
#include "Renderer/Renderer.h"
#include "GameObject.h"
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
		void loadGameObjects();

		LimaWindow m_LimaWindow{ WIDTH, HEIGHT, "Lima Engine" };
		LimaDevice m_LimaDevice{ m_LimaWindow };
		LimaRenderer m_LimaRenderer{ m_LimaWindow, m_LimaDevice };

		std::vector<LimaGameObject> m_GameObjects{};
	};

	Application* CreateApplication();
} // namespace Lima


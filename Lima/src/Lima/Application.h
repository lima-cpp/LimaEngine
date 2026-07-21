#pragma once

#include "Window.h"
#include "Pipeline.h"
#include "LimaDevice.h"
#include "Core.h"

namespace Lima {

	class LIMA_API Application
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		Application();
		virtual ~Application();

		void Run();

	private:
		LimaWindow LimaWindow{ WIDTH, HEIGHT, "Lima Engine" };
		LimaDevice LimaDevice{ LimaWindow };
		LimaPipeline LimaPipeline{
			LimaDevice,
			"C:/LimaEngine/Lima/src/Lima/Shaders/simple_shader.vert.spv",
			"C:/LimaEngine/Lima/src/Lima/Shaders/simple_shader.frag.spv",
			LimaPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
	};

	Application* CreateApplication();
} // namespace Lima


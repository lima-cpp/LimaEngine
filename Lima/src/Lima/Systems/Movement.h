#pragma once

#include "../GameObject.h"
#include "../Vulkan/Window.h"

namespace Lima {
	class MovementSystem {

	public:
		struct KeyMappings {
			int moveForward = GLFW_KEY_W;
			int moveBackward = GLFW_KEY_S;
			int moveLeft = GLFW_KEY_A;
			int moveRight = GLFW_KEY_D;
			int moveUp = GLFW_KEY_E;
			int moveDown = GLFW_KEY_Q;
			int lookUp = GLFW_KEY_UP;
			int lookDown = GLFW_KEY_DOWN;
			int lookLeft = GLFW_KEY_LEFT;
			int lookRight = GLFW_KEY_RIGHT;

		};

		void moveInPlaneXZ(GLFWwindow* window, float deltaTime, LimaGameObject& gameObject);

		KeyMappings keys{};
		float moveSpeed{ 3.f };
		float lookSpeed{ 1.5f };
	};
}
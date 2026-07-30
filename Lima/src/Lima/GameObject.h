#pragma once

#include "Model.h"

#include <memory>

namespace Lima {

	struct Transform2DComponent {
		glm::vec2 translation{};
		glm::vec2 scale{ 1.f, 1.f };
		float rotation;

		glm::mat2 mat2() { 
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotMatrix{ {c, s}, {-s, c} };
			glm::mat2 scaleMat{ {scale.x, .0f}, {.0f, scale.y} };

			return rotMatrix * scaleMat;
		}
	};


	class LimaGameObject {
	public:
		using id_t = unsigned int;

		static LimaGameObject createGameObject() {
			static id_t currentId = 0;
			return LimaGameObject{ currentId++ };
		}

		LimaGameObject(const LimaGameObject&) = delete;
		LimaGameObject& operator = (const LimaGameObject&) = delete;
		LimaGameObject(LimaGameObject&&) = default;
		LimaGameObject& operator = (LimaGameObject&&) = default;

		id_t getId() { return id; }

		std::shared_ptr<LimaModel> model{};
		glm::vec3 color{};
		Transform2DComponent transform2D{};

	private:
		LimaGameObject(id_t objId) : id{objId} {}

		id_t id;
	};
}


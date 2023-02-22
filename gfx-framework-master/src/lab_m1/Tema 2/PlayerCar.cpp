#include "PlayerCar.h"

#include <vector>
#include <random>

#include "core/engine.h"
#include "utils/gl_utils.h"

namespace object {

	PlayerCar::PlayerCar(glm::vec3 startingPosition, glm::vec3 startingDirection) {
		position = startingPosition;
		direction = glm::normalize(startingDirection);
	}

	float PlayerCar::calculateAngleOfCar() {
		float angle = acos(glm::dot(glm::vec3(1, 0, 0), direction));

		if (direction.z > 0) {
			angle = 2 * M_PI - angle;
		}

		return angle;
	}

	void PlayerCar::moveCar(float speed) {
		position = position + speed * direction;
	}

	void PlayerCar::rotateCar(float speed) {
		float angle = 2 * M_PI - calculateAngleOfCar();
		angle += speed;
		float z = sin(angle);
		float x = cos(angle);

		direction = glm::normalize(glm::vec3(x, 0, z));
	}
}
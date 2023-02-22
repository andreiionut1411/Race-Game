#include "Car.h"

#include <vector>
#include <random>

#include "core/engine.h"
#include "utils/gl_utils.h"

namespace object {

	Car::Car(int point, glm::vec3 col, bool randomOffsetCar) {
		pointIndex = point;
		color = col;

		if (randomOffsetCar) {
			randomPosition();
		}
   }

	void Car::positionCarOnRoad(object::Road road) {
		glm::vec3 p;
		// If we finished a lap, then we reset the index of the current point.
		if (pointIndex <= 0 && speed < 0) {
			pointIndex += (road.verticesInitial.size() - 2) * road.numberOfVerticesPerSegment;
		}
		else if (pointIndex >= (road.verticesInitial.size() - 2) * road.numberOfVerticesPerSegment && speed > 0) {
			pointIndex -= (road.verticesInitial.size() - 2) * road.numberOfVerticesPerSegment;
			lap++;
		}

		position = road.denseVertices[(int)pointIndex].position;
		direction = road.directionVector[(int)pointIndex];

		// We make the car to go at a certain distance from the middle of the road.
		p = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));
		position += distanceFromCenterOfRoad * p;
	}

	// The car moves from one intermiediate vertex to another, and pointIndex keeps track of
	// the current index.
	void Car::moveCar(float time) {
		pointIndex += speed * time;
	}

	float Car::calculateAngleOfCar() {
		float angle = acos(glm::dot(glm::vec3(1, 0, 0), glm::normalize(direction)));

		if (direction.z > 0) {
			angle = 2 * M_PI-angle;
		}

		if (speed > 0) {
			angle += M_PI;
		}

		return angle;
	}

	void Car::randomPosition() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(0, 50);
		int x = distr(gen);

		x -= 25;

		distanceFromCenterOfRoad = x / 100.0;
	}
}
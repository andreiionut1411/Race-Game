#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object
{
    class PlayerCar {
    public:
        glm::vec3 position;
        glm::vec3 direction; // The direction will always be a normalised vector

        PlayerCar(glm::vec3 position, glm::vec3 direction);
        float calculateAngleOfCar();
        void moveCar(float speed);
        void rotateCar(float speed);
    };
}

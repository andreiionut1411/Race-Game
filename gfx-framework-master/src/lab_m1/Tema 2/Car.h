#pragma once

#include <string>
#include "Road.h"

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object
{
    class Car {
    public:
        float pointIndex;
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 color;
        float distanceFromCenterOfRoad = 0;
        int speed;
        int lap = 0;

        Car(int point, glm::vec3 color, bool randomOffsetCar);
        void positionCarOnRoad(object::Road road);
        void moveCar(float time);
        float calculateAngleOfCar();
        void randomPosition();
    };
}

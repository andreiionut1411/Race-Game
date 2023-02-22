#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object
{
    class Road {
    private:
        glm::vec3 color = glm::vec3(0, 0, 0);
        float sideOfBranches;

    public:

        Road(float sideOfBranches);

        const int numberOfVerticesPerSegment = 200;
        const float upDist = 0.6, downDist = 0.6;
        std::vector<VertexFormat> denseVertices;
        std::vector<VertexFormat> vertices;
        std::vector<glm::vec3> trees;
        std::vector<glm::vec3> directionVector;
        // Declaring each vertex of the the Spa-Francorchamps race track.
        std::vector<VertexFormat> verticesInitial =
        {
            VertexFormat(glm::vec3(-15.8f, 0.0f, 8.6f), color), // A
            VertexFormat(glm::vec3(-18.3f, 0.0f, 11.03f), color),
            VertexFormat(glm::vec3(-20.19f, 0.0f, 12.87f), color),
            VertexFormat(glm::vec3(-22.19f, 0.0f, 14.76f), color),
            VertexFormat(glm::vec3(-23.56f, 0.0f, 15.16f), color),
            VertexFormat(glm::vec3(-23.97f, 0.0f, 14.7f), color), // T4
            VertexFormat(glm::vec3(-24.2f, 0.0f, 14.2f), color), // U4
            VertexFormat(glm::vec3(-24.34f, 0.0f, 13.09f), color),
            VertexFormat(glm::vec3(-23.56f, 0.0f, 10.98f), color),
            VertexFormat(glm::vec3(-22.75f, 0.0f, 9.14f), color),
            VertexFormat(glm::vec3(-21.54f, 0.0f, 6.89f), color),
            VertexFormat(glm::vec3(-20.5f, 0.0f, 6.13f), color),
            VertexFormat(glm::vec3(-19.38f, 0.0f, 4.51f), color),
            VertexFormat(glm::vec3(-17.57f, 0.0f, 2.18f), color),
            VertexFormat(glm::vec3(-17.21f, 0.0f, 1.3f), color), // V4
            VertexFormat(glm::vec3(-16.92f, 0.0f, 0.76f), color), // M
            VertexFormat(glm::vec3(-16.2f, 0.0f, 0.16f), color), // W4
            VertexFormat(glm::vec3(-15.39f, 0.0f, -0.43f), color),
            VertexFormat(glm::vec3(-14.21f, 0.0f, -0.84f), color),
            VertexFormat(glm::vec3(-12.36f, 0.0f, -2.51f), color),
            VertexFormat(glm::vec3(-9.84f, 0.0f, -4.58f), color),
            VertexFormat(glm::vec3(-8.86f, 0.0f, -5.75f), color),
            VertexFormat(glm::vec3(-6.43f, 0.0f, -7.09f), color),
            VertexFormat(glm::vec3(-3.37f, 0.0f, -8.49f), color),
            VertexFormat(glm::vec3(-1.48f, 0.0f, -9.57f), color),
            VertexFormat(glm::vec3(1.0f, 0.0f, -11.0f), color),
            VertexFormat(glm::vec3(3.11f, 0.0f, -12.09f), color),
            VertexFormat(glm::vec3(5.26f, 0.0f, -13.3f), color),
            VertexFormat(glm::vec3(7.24f, 0.0f, -14.33f), color),//A1
            VertexFormat(glm::vec3(9.18f, 0.0f, -14.69f), color),
            VertexFormat(glm::vec3(10.03f, 0.0f, -13.97f), color),
            VertexFormat(glm::vec3(10.84f, 0.0f, -13.3f), color),
            VertexFormat(glm::vec3(11.79f, 0.0f, -13.12f), color),
            VertexFormat(glm::vec3(12.82f, 0.0f, -13.43f), color),
            VertexFormat(glm::vec3(13.67f, 0.0f, -14.74f), color),
            VertexFormat(glm::vec3(14.75f, 0.0f, -15.46f), color),
            VertexFormat(glm::vec3(15.7f, 0.0f, -15.5f), color),
            VertexFormat(glm::vec3(16.33f, 0.0f, -15.14f), color),
            VertexFormat(glm::vec3(16.82f, 0.0f, -14.56f), color),
            VertexFormat(glm::vec3(17.8f, 0.0f, -13.8f), color),
            VertexFormat(glm::vec3(19.16f, 0.0f, -12.85f), color), // M1
            VertexFormat(glm::vec3(20.73f, 0.0f, -12.04f), color),
            VertexFormat(glm::vec3(22.35f, 0.0f, -11.32f), color),
            VertexFormat(glm::vec3(23.34f, 0.0f, -10.06f), color),
            VertexFormat(glm::vec3(23.88f, 0.0f, -9.79f), color),
            VertexFormat(glm::vec3(24.29f, 0.0f, -8.76f), color),
            VertexFormat(glm::vec3(24.47f, 0.0f, -7.63f), color),
            VertexFormat(glm::vec3(24.06f, 0.0f, -6.6f), color),
            VertexFormat(glm::vec3(23.07f, 0.0f, -6.64f), color),
            VertexFormat(glm::vec3(22.17f, 0.0f, -7.0f), color),
            VertexFormat(glm::vec3(21.45f, 0.0f, -7.77f), color),
            VertexFormat(glm::vec3(20.6f, 0.0f, -8.2f), color),
            VertexFormat(glm::vec3(19.65f, 0.0f, -8.98f), color), // A2
            VertexFormat(glm::vec3(18.58f, 0.0f, -9.07f), color),
            VertexFormat(glm::vec3(17.8f, 0.0f, -8.2f), color),
            VertexFormat(glm::vec3(16.96f, 0.0f, -7.68f), color),
            VertexFormat(glm::vec3(16.6f, 0.0f, -6.91f), color),
            VertexFormat(glm::vec3(15.79f, 0.0f, -5.66f), color),
            VertexFormat(glm::vec3(14.89f, 0.0f, -4.94f), color),
            VertexFormat(glm::vec3(13.89f, 0.0f, -3.76f), color),
            VertexFormat(glm::vec3(12.2f, 0.0f, -2.6), color),
            VertexFormat(glm::vec3(10.8f, 0.0f, -1.7f), color),
            VertexFormat(glm::vec3(10.12f, 0.0f, -1.11f), color),
            VertexFormat(glm::vec3(9.67f, 0.0f, -0.26f), color),
            VertexFormat(glm::vec3(10.03f, 0.0f, 0.69f), color), // M2
            VertexFormat(glm::vec3(10.66f, 0.0f, 1.72f), color),
            VertexFormat(glm::vec3(11.38f, 0.0f, 2.75f), color),
            VertexFormat(glm::vec3(12.2f, 0.0f, 3.0f), color),
            VertexFormat(glm::vec3(13.71f, 0.0f, 3.0f), color),
            VertexFormat(glm::vec3(15.48f, 0.0f, 3.0f), color),
            VertexFormat(glm::vec3(16.87f, 0.0f, 3.25f), color),
            VertexFormat(glm::vec3(18.93f, 0.0f, 3.56f), color),
            VertexFormat(glm::vec3(19.83f, 0.0f, 3.88f), color),
            VertexFormat(glm::vec3(20.46f, 0.0f, 4.96f), color),
            VertexFormat(glm::vec3(19.97f, 0.0f, 5.68f), color),
            VertexFormat(glm::vec3(19.29f, 0.0f, 6.49f), color),
            VertexFormat(glm::vec3(19.2f, 0.0f, 7.43f), color), // A3
            VertexFormat(glm::vec3(19.56f, 0.0f, 8.24f), color),
            VertexFormat(glm::vec3(21.09f, 0.0f, 8.82f), color),
            VertexFormat(glm::vec3(22.04f, 0.0f, 9.27f), color),
            VertexFormat(glm::vec3(23.21f, 0.0f, 9.86f), color),
            VertexFormat(glm::vec3(23.7f, 0.0f, 10.62f), color),
            VertexFormat(glm::vec3(23.79f, 0.0f, 11.79f), color),
            VertexFormat(glm::vec3(23.52f, 0.0f, 12.92f), color),
            VertexFormat(glm::vec3(22.98f, 0.0f, 14.36f), color),
            VertexFormat(glm::vec3(22.4f, 0.0f, 15.16f), color),
            VertexFormat(glm::vec3(20.6f, 0.0f, 15.52f), color),
            VertexFormat(glm::vec3(19.07f, 0.0f, 14.98f), color),
            VertexFormat(glm::vec3(17.14f, 0.0f, 14.04f), color), // M3
            VertexFormat(glm::vec3(16.37f, 0.0f, 13.32f), color),
            VertexFormat(glm::vec3(14.35f, 0.0f, 11.75f), color),
            VertexFormat(glm::vec3(13.27f, 0.0f, 10.67f), color),
            VertexFormat(glm::vec3(12.2f, 0.0f, 8.6f), color),
            VertexFormat(glm::vec3(10.26f, 0.0f, 7.39f), color),
            VertexFormat(glm::vec3(8.77f, 0.0f, 5.99f), color),
            VertexFormat(glm::vec3(7.51f, 0.0f, 5.05f), color),
            VertexFormat(glm::vec3(6.47f, 0.0f, 3.38f), color),
            VertexFormat(glm::vec3(4.49f, 0.0f, 2.16f), color),
            VertexFormat(glm::vec3(2.8f, 0.0f, 1.46f), color),
            VertexFormat(glm::vec3(1.1f, 0.0f, 1.15f), color),
            VertexFormat(glm::vec3(-0.51f, 0.0f, 0.64f), color), // A4
            VertexFormat(glm::vec3(-1.89f, 0.0f, 0.55f), color),
            VertexFormat(glm::vec3(-3.13f, 0.0f, 1.23f), color),
            VertexFormat(glm::vec3(-4.69f, 0.0f, 1.88f), color),
            VertexFormat(glm::vec3(-5.9f, 0.0f, 2.69f), color),
            VertexFormat(glm::vec3(-6.82f, 0.0f, 3.56f), color),
            VertexFormat(glm::vec3(-8.09f, 0.0f, 4.19f), color),
            VertexFormat(glm::vec3(-8.94f, 0.0f, 5.54f), color),
            VertexFormat(glm::vec3(-9.39f, 0.0f, 6.35f), color),
            VertexFormat(glm::vec3(-9.93f, 0.0f, 7.16f), color),
            VertexFormat(glm::vec3(-10.65f, 0.0f, 7.07f), color),
            VertexFormat(glm::vec3(-11.15f, 0.0f, 6.35f), color),
            VertexFormat(glm::vec3(-11.15f, 0.0f, 5.36f), color), // M4
            VertexFormat(glm::vec3(-11.6f, 0.0f, 4.69f), color),
            VertexFormat(glm::vec3(-12.41f, 0.0f, 4.87f), color),
            VertexFormat(glm::vec3(-12.95f, 0.0f, 5.41f), color),
            VertexFormat(glm::vec3(-13.76f, 0.0f, 6.17f), color),
            VertexFormat(glm::vec3(-14.3f, 0.0f, 6.98f), color),
            VertexFormat(glm::vec3(-14.84f, 0.0f, 7.61f), color), // S4
            VertexFormat(glm::vec3(-15.8f, 0.0f, 8.6f), color),
            VertexFormat(glm::vec3(-18.3f, 0.0f, 11.03f), color)// We add the first vertex again to get a loop
        };

        // Create square with given bottom left corner, length and color
        Mesh* CreateRoad(const std::string& name);
        void createDenseVertices(); // Creating more vertices from initial vertices.
        void createVertices(); // Creating the vertices that will be used as the sides of the road.
        void removeTreesOnTrack();

        // The method verifies if the given point is further to the road than a certain distance.
        // It will be used to know if the tree is on the road or not. minDistance represents the
        // radius of the trees branches.
        bool isOnRoad(float positionX, float positionZ, float minDistance);

        // For the trees we can have a set number of possible locations, and we give each one a 65%
        // chance of actually using that position.
        bool renderTreeRandomiser();

        // The trees will be at a random distance from the track.
        float distanceFromTrackRandomiser();
    };
}

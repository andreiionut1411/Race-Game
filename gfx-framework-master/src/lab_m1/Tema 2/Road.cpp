#include "Road.h"

#include <vector>
#include <random>

#include "core/engine.h"
#include "utils/gl_utils.h"

namespace object {

    Road::Road(float side) {
        sideOfBranches = side;
        createDenseVertices();
        createVertices();
        removeTreesOnTrack();
    }

    void Road::createDenseVertices() {
        glm::vec3 p1, p2, d;

        for (int i = 0; i < verticesInitial.size() - 1; i++) {
            p1 = verticesInitial[i].position;
            p2 = verticesInitial[i + 1].position;
            d = p2 - p1;

            // We create more vertices starting from the initial points
            for (int j = 0; j < numberOfVerticesPerSegment; j++) {
                float distance = ((float)j) / numberOfVerticesPerSegment;
                denseVertices.push_back(VertexFormat(p1 + distance * d, color));
                directionVector.push_back(d);
            }
        }

        denseVertices.push_back(verticesInitial[verticesInitial.size() - 1]);
        directionVector.push_back(d);
    }

    void Road::createVertices() {
        glm::vec3 p1, p2, p, d;

        for (int i = 0; i < (verticesInitial.size() - 1) * numberOfVerticesPerSegment; i++) {
            p1 = denseVertices[i].position;
            p2 = denseVertices[i + 1].position;
            d = p2 - p1;
            p = glm::normalize(glm::cross(d, glm::vec3(0, 1, 0)));

            vertices.push_back(VertexFormat(p1 + upDist * p, glm::vec3(0, 0, 0)));
            vertices.push_back(VertexFormat(p1 - downDist * p, glm::vec3(0.1, 0.1, 0.1)));

            // We add the trees' locations and we actually use them if the randomiser chooses so.
            // They will also be at a random distance from the track.
            if (i % numberOfVerticesPerSegment == 0) {
                if (renderTreeRandomiser()) {
                    trees.push_back(p1 + (float)(upDist + distanceFromTrackRandomiser()) * p);
                }

                if (renderTreeRandomiser()) {
                    trees.push_back(p1 - (float)(downDist + distanceFromTrackRandomiser()) * p);
                }
            }

            // For every vertex that was in the initial pool, we have 2 pairs of up and lower new vertices
            if ((i + 1) % verticesInitial.size() == 0) {
                vertices.push_back(VertexFormat(p2 + upDist * p, glm::vec3(0, 0, 0)));
                vertices.push_back(VertexFormat(p2 - downDist * p, glm::vec3(0.1, 0.1, 0.1)));
            }
        }
    }

    bool Road::isOnRoad(float positionX, float positionZ, float minDistance) {
        for (int i = 0; i < vertices.size(); i++) {
            if (glm::distance(vertices[i].position, glm::vec3(positionX, 0, positionZ)) < minDistance) {
                return true;
            }
        }

        return false;
    }

    bool Road::renderTreeRandomiser() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(1, 100);
        int x = distr(gen);

        if (x <= 65) return true;

        return false;
    }

    float Road::distanceFromTrackRandomiser() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(50, 100);
        int x = distr(gen);

        return x / 100.0;
    }

    void Road::removeTreesOnTrack() {
        std::vector<glm::vec3> aux;
        bool overlap;

        for (int i = 0; i < trees.size(); i++) {
            overlap = false;
            
            if (!isOnRoad(trees[i].x, trees[i].z, sideOfBranches * sqrt(2))) {

                // We verify if 2 trees overlap
                for (int j = 0; j < aux.size(); j++) {
                    if (glm::distance(trees[i], aux[j]) <= 2 * sideOfBranches * sqrt(2)) {
                        overlap = true;
                        break;
                    }
                }
                
                if (!overlap) {
                    aux.push_back(trees[i]);
                }
            }
        }

        trees.clear();

        for (int i = 0; i < aux.size(); i++) {
            trees.push_back(aux[i]);
        }
    }

    Mesh* Road::CreateRoad(const std::string& name)
    {
        Mesh* road = new Mesh(name);
        std::vector<unsigned int> indices;

        for (int i = 0; i < vertices.size() / 2 - 1; i++) {
            indices.push_back(i * 2);
            indices.push_back(i * 2 + 1);
            indices.push_back(i * 2 + 3);

            indices.push_back(i * 2);
            indices.push_back(i * 2 + 3);
            indices.push_back(i * 2 + 2);
        }

        road->InitFromData(vertices, indices);
        return road;
    }

}
#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"
#include "lab_m1/Tema 2/camera.h"
#include "lab_m1/Tema 2/Road.h"
#include "lab_m1/Tema 2/Car.h"
#include "lab_m1/Tema 2/PlayerCar.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderTrack();
        void RenderTree(float positionX, float positionZ);
        void RenderAllTrees();
        glm::vec3 generateRandomColor();
        void generateStartingPointCars();
        void generateRaceStartingPointCars();
        void InitializeParmeters();
        void InitializeGame();
        void InitializeClassic();
        void InitializeRace();
        void placePlayerCarRace(int index);
        void RenderCars();
        void moveCars(float time);
        bool hasCrashed();
        void movePlayerCar(float speed);
        void RenderPlayerCar();
        void RenderFinishLine();
        void positionCamera();
        void positionMinimapCamera();
        void RenderMiniMap();
        void RenderScene();
        void createCheckPoints();
        void RenderHUD();
        bool verifyStart();
        int findPlace();
        std::string placeToFormat();
        std::string timeToFormat(float time);

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3& color);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        camera::Camera *camera;
        glm::mat4 projectionMatrix;

        // Tree parameters
        const float sideOfBranches = 0.45;
        const float branchHeight = 1.2;
        const float trunkHeight = 0.8;
        const float trunkSide = 0.3;

        // TODO(student): If you need any other class variables, define them here.
        float fov = RADIANS(60);
        float bottom = -5.0f;
        float top = 5.0f;
        object::Road road = object::Road::Road(sideOfBranches);
        std::vector<object::Car> cars;

        // We initialize the player's car at the start of the track
        const glm::vec3 startPosition = road.verticesInitial[0].position;
        const glm::vec3 startDirection = road.directionVector[0];
        object::PlayerCar playerCar = object::PlayerCar::PlayerCar(startPosition, startDirection);
        float playerAcceleration = 2.5;
        float playerInertia = 0.7;
        float playerSpeed = 0;
        float playerSpeedMax = 7;
        float playerRotationSpeed = 1;
        float heightOfCamera = 0.5;
        float distanceToCar = 1;
        glm::mat4 playerModelMatrix;
        int closestPoint; // This is the closest point from the center of the road to the player's car
        
        // Declaring minimap dimensions and position
        float minimapX, minimapY;
        float minimapHeight, minimapWidth;
        float const minimapCameraHeight = 10;

        // Parameters for laps.
        int lap = 1;
        const int totalLaps = 3;
        float totalTime;
        float bestLapTime;
        float lapTime = 0;
        float timeUntilStart;
        float goTime; // The time the start message is shown.
        float penaltyTime;
        const int numberOfCheckpoints = 5;
        int checkpoints[5];
        int lastCheckpointPassed;
        bool raceIsOn = false;
        int showMenu;
        const float raceOffset = 0.3;
        int gameMode = 1; // 1 for classic, 2 for race.

        // Text renderer
        gfxc::TextRenderer* textRenderer;
    };
}   // namespace m1

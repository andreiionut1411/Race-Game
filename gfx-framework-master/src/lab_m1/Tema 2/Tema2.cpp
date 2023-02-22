#include "lab_m1/Tema 2/Tema2.h"
#include "lab_m1/Tema 2/Road.h"
#include <iomanip>

#include <vector>
#include <string>
#include <iostream>
#include <random>

using namespace std;
using namespace m1;

#define BLACK glm::vec3(0, 0, 0)
#define GREEN glm::vec3(0, 0.753, 0.18)
#define LEAF_GREEN glm::vec3(0.4, 1, 0.4)
#define BROWN glm::vec3(0.502, 0.443, 0.325)
#define CYAN glm::vec3(0.208, 0.318, 0.361)
#define NO_COLOR glm::vec3(-1, -1, -1)
#define CLASSIC 1
#define RACE 2

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    camera = new camera::Camera();
    positionCamera();

    {
        Mesh* mesh = new Mesh("ground");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = road.CreateRoad("road");
        AddMeshToList(mesh);
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("car");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "Tema2"), "untitled.fbx");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("police");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "Tema2"), "police.fbx");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("finish");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "Tema2"), "finish.fbx");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("MyShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema 2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema 2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // We first start with rendering the menu.
    showMenu = 1;
    textRenderer = new gfxc::TextRenderer(window->props.selfDir, window->GetResolution().x, window->GetResolution().y);
    textRenderer->Load(window->props.selfDir + "\\assets\\fonts\\Hack-Bold.ttf", 128);

    // We prepare the checkpoints system
    createCheckPoints();
}

void Tema2::InitializeGame() {
    if (gameMode == CLASSIC) InitializeClassic();
    else InitializeRace();
}

void Tema2::InitializeParmeters() {
    lap = 1;
    totalTime = 0.0f;
    bestLapTime = 0.0f;
    lapTime = 0.0f;
    timeUntilStart = 3.0;
    goTime = 1.0;
    penaltyTime = 0.0;
    lastCheckpointPassed = -1;
    raceIsOn = true;
    showMenu = 0;
    playerSpeed = 0;
}

void Tema2::InitializeClassic() {
    InitializeParmeters();
    generateStartingPointCars();
    playerCar.position = startPosition;
    playerCar.direction = glm::normalize(startDirection);
}

void Tema2::generateRaceStartingPointCars() {
    cars.clear();

    for (int i = 0; i < 7; i++) {
        cars.push_back(object::Car((road.verticesInitial.size() - 2) * road.numberOfVerticesPerSegment - 120 * (i + 1), generateRandomColor(), false));
        cars[i].speed = 458 - 40 * i; // The first cars will have a higher speed
        cars[i].distanceFromCenterOfRoad = (i % 2 == 0) ? raceOffset : -raceOffset;
    }
}

void Tema2::placePlayerCarRace(int pointIndex) {
    glm::vec3 position = road.denseVertices[pointIndex].position;
    glm::vec3 direction = road.directionVector[pointIndex];

    // We make the car to go at a certain distance from the middle of the road.
    glm::vec3 p = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));
    position += -raceOffset * p;

    playerCar.position = position;
    playerCar.direction = glm::normalize(direction);
}

void Tema2::InitializeRace() {
    InitializeParmeters();
    generateRaceStartingPointCars();
    int index = (road.verticesInitial.size() - 2) * road.numberOfVerticesPerSegment - 120 * 8;
    lap = 0;
    lastCheckpointPassed = 3;
    placePlayerCarRace(index);
    closestPoint = index;
}

int Tema2::findPlace() {
    for (int i = 6; i >= 0; i--) {
        if (lap > cars[i].lap && cars[i].lap != 0) continue;
        else if (cars[i].lap == 0 && closestPoint < checkpoints[0]) continue; // the player passed the line initially, and the car not
        else if (cars[i].lap == 0 && closestPoint >= (int)cars[i].pointIndex) continue; // the player is behind the opponent
        else if (cars[i].lap == 0 && closestPoint < (int)cars[i].pointIndex) return i + 2;
        else if (((int)cars[i].pointIndex > closestPoint && cars[i].lap == lap) || lap < cars[i].lap) {
            return i + 2;
        }
    }

    return 1;
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.49, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
    minimapHeight = resolution.y / 5.0;
    minimapWidth = resolution.x / 5.0;
    minimapX = 63 * resolution.x / 80.0;
    minimapY = resolution.y / 15.0;
}

// We render a tree with the center at the given position (in the XOZ plane)
void Tema2::RenderTree(float positionX, float positionZ) {
    glm::mat4 aux;
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(positionX, trunkHeight / 2 - 0.1, positionZ));
    aux = modelMatrix;

    modelMatrix = glm::scale(modelMatrix, glm::vec3(trunkSide, trunkHeight, trunkSide));
    RenderMesh(meshes["box"], shaders["MyShader"], modelMatrix, BROWN);

    modelMatrix = aux;
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, trunkHeight / 2 + 0.2, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(sideOfBranches * 2, branchHeight, sideOfBranches * 2));
    RenderMesh(meshes["box"], shaders["MyShader"], modelMatrix, LEAF_GREEN);
}

void Tema2::RenderAllTrees() {
    for (int i = 0; i < road.trees.size(); i++) {
        RenderTree(road.trees[i].x, road.trees[i].z);
    }
}

void Tema2::generateStartingPointCars() {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, road.denseVertices.size() / 6);
    int x = distr(gen);
    cars.clear();

    // We will have 6 moving cars on track.
    for (int i = 0; i < 6; i++) {
        cars.push_back(object::Car(x + i * road.denseVertices.size() / 6, generateRandomColor(), true));
        cars[i].speed = -300;
    }
}

glm::vec3 Tema2::generateRandomColor() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 255);

    float red = distr(gen) / 255.0;
    float green = distr(gen) / 255.0;
    float blue = distr(gen) / 255.0;

    return glm::vec3(red, green, blue);
}

void Tema2::RenderCars() {
    for (int i = 0; i < cars.size(); i++) {
        cars[i].positionCarOnRoad(road);
        object::Car car = cars[i];

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(car.position.x, 0.1, car.position.z));
        modelMatrix = glm::rotate(modelMatrix, car.calculateAngleOfCar(), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4, 0.2, 0.2));

        // Scaling down the car mesh.
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.5, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.8, 0.6));

        // For the police car, we will be basing the color on the model. 
        // But the shader needs a color to be based as uniform, so for this
        // we pass a vec3 that is NOT a color, so that we know in the mesh to
        // render it as we would render the car normally.
        if (gameMode == CLASSIC) {
            RenderMesh(meshes["police"], shaders["MyShader"], modelMatrix, NO_COLOR);
        }
        // For the race, every car will be the racing type car.
        else {
            RenderMesh(meshes["car"], shaders["MyShader"], modelMatrix, NO_COLOR);
        }
    }
}

void Tema2::RenderPlayerCar() {
    // Placing the car on the track.
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(playerCar.position.x, 0, playerCar.position.z));
    modelMatrix = glm::rotate(modelMatrix, playerCar.calculateAngleOfCar(), glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4, 0.2, 0.2));

    // Scaling down the car mesh.
    modelMatrix = glm::rotate(modelMatrix, (float)M_PI, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.8, 0.6));
    playerModelMatrix = modelMatrix;
    RenderMesh(meshes["car"], shaders["MyShader"], modelMatrix, NO_COLOR);
}

// The finish line is placed on the first vertex of the road.
void Tema2::RenderFinishLine() {
    float angle = acos(glm::dot(glm::vec3(1, 0, 0), glm::normalize(road.directionVector[0])));

    if (road.directionVector[0].z > 0) {
        angle = 2 * M_PI - angle;
    }

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(road.verticesInitial[0].position.x, 0.02, road.verticesInitial[0].position.z));
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));

    // We rotate with 3 * pi / 2 because the model of the finish line has an empty space, so if we
    // rotate with only pi / 2, then the finish line would appear behind the car.
    modelMatrix = glm::rotate(modelMatrix, (float)(3 * M_PI) / 2, glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, (float)M_PI / 2, glm::vec3(1, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.4, 1));
    RenderMesh(meshes["finish"], shaders["MyShader"], modelMatrix, NO_COLOR);
}

void Tema2::moveCars(float time) {
    for (int i = 0; i < cars.size(); i++) {
        cars[i].moveCar(time);
    }
}

void Tema2::RenderTrack() {
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5, 1, 1.5));

    // Render the grass.
    RenderMesh(meshes["ground"], shaders["MyShader"], modelMatrix, GREEN);

    // We render the road above the grass, but insignificantly higher.
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01, 0));
    RenderMesh(meshes["road"], shaders["MyShader"], modelMatrix, BLACK);
}

bool Tema2::hasCrashed() {
    bool hasCrashed = false;

    for (int i = 0; i < cars.size(); i++) {
        if (glm::distance(playerCar.position, cars[i].position) < 0.2) {
            hasCrashed = true;
            break;
        }
    }

    return hasCrashed;
}

void Tema2::movePlayerCar(float speed) {
    glm::vec3 position = playerCar.position;
    bool isOnTrack = false;
    
    playerCar.moveCar(speed);
    glm::vec3 newPosition = playerCar.position;

    // If the car is moving, we verify if the next position will be inside the track or not.
    // If the distance between the car and any other point from the middle of the track is
    // always larger than half the width of the street, then it means it is out of bounds, so
    // we stop the car and don't move it further.
    if (speed != 0) {
        float distance;
        for (int i = 0; i < road.denseVertices.size(); i++) {
            if (glm::distance(newPosition, road.denseVertices[i].position) < road.upDist) {
                distance = glm::distance(newPosition, road.denseVertices[i].position);
                closestPoint = i;

                if (i != 0) {
                    while (glm::distance(newPosition, road.denseVertices[i + 1].position) <= distance) {
                        distance = glm::distance(newPosition, road.denseVertices[i + 1].position);
                        i++;
                        closestPoint = i;
                    }
                }
                else {
                    float newDistance = glm::distance(newPosition, road.denseVertices[0].position);
                    int newClosestPoint = closestPoint;
                    while (glm::distance(newPosition, road.denseVertices[i + 1].position) <= distance) {
                        distance = glm::distance(newPosition, road.denseVertices[i + 1].position);
                        i++;
                        closestPoint = i;
                    }

                    // If we find that the player's car is in the radius of the first point on the track,
                    // we also verify if there isn't another point before the lap that is closer to the car.
                    // This way, the player won't cross the finish line prematurely.
                    int index = (road.verticesInitial.size() - 2) * road.numberOfVerticesPerSegment;
                    while (glm::distance(newPosition, road.denseVertices[index - 1].position) <= newDistance) {
                        index--;
                        newDistance = glm::distance(newPosition, road.denseVertices[index].position);
                        newClosestPoint = index;
                    }

                    if (newDistance <= distance) closestPoint = newClosestPoint;
                }

                // We verify where we are on the track based on the checkpoints
                if (lastCheckpointPassed < 2) {
                    if (closestPoint >= checkpoints[lastCheckpointPassed + 1] && closestPoint <= checkpoints[lastCheckpointPassed + 2]) {
                        lastCheckpointPassed++;
                    }
                    else {
                        // If we start going backwards, we decrement the last checkpoint, so we don't let some funny bussiness to happen
                        if (lastCheckpointPassed != -1) {
                            if (checkpoints[lastCheckpointPassed] >= closestPoint) {
                                lastCheckpointPassed--;
                            }
                        }
                    }
                }
                else if (lastCheckpointPassed == 2) {
                    if (closestPoint >= checkpoints[lastCheckpointPassed + 1]) {
                        lastCheckpointPassed++;
                    }
                    else if (closestPoint <= checkpoints[lastCheckpointPassed]) {
                        lastCheckpointPassed--;
                    }
                }
                else {
                    if (closestPoint >= 5 && closestPoint <= checkpoints[0]) {
                        lastCheckpointPassed = -1;
                        lap++;

                        if (bestLapTime == 0.0f && lap > 1) {
                            bestLapTime = lapTime;
                        }
                        else if (bestLapTime > lapTime) {
                            bestLapTime = lapTime;
                        }

                        if (lap > 1) lapTime = 0;
                        if (lap > 1) lapTime = 0;
                        if (lap > totalLaps) {
                            raceIsOn = false;
                            showMenu = 3;
                        }
                    }
                    else if (closestPoint <= checkpoints[lastCheckpointPassed] && closestPoint >= checkpoints[0]) {
                        lastCheckpointPassed--;
                    }
                }
                isOnTrack = true;

                break;
            }
        }

        if (!isOnTrack || hasCrashed()) {
            playerCar.position = position;
            playerSpeed = 0;
        }
    }
}

void Tema2::positionCamera() {
    camera->Set(glm::vec3(playerCar.position.x - playerCar.direction.x * distanceToCar, heightOfCamera, playerCar.position.z - playerCar.direction.z * distanceToCar), playerCar.position, glm::vec3(0, 1, 0));
    projectionMatrix = glm::perspective(fov, window->props.aspectRatio, 0.01f, 200.0f);
}

void Tema2::positionMinimapCamera() {
    camera->Set(glm::vec3(playerCar.position.x, minimapCameraHeight, playerCar.position.z), playerCar.position, glm::vec3(1, 0, 0));
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -20.0f, 20.0f);
}

void Tema2::RenderMiniMap() {
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(minimapX, minimapY, minimapWidth, minimapHeight);
}

void Tema2::RenderScene() {
    RenderTrack();
    RenderFinishLine();
    RenderAllTrees();
    RenderCars();
    RenderPlayerCar();
}

// We generate the indexes of the checkpoints based on the denseVertices
void Tema2::createCheckPoints() {
    int segment = (road.verticesInitial.size() - 2) * road.numberOfVerticesPerSegment / 5;

    for (int i = 0; i < 4; i++) {
        checkpoints[i] = segment * (i + 1);
    }

    checkpoints[4] = 0; // The last checkpoint is the finish line, which is actually the first vertex from the road.
}

string Tema2::timeToFormat(float time) {
    int minutes = (int)(time / 60);
    float seconds = time - minutes * 60;
    string minute_string;

    if (minutes < 10) {
        minute_string = "0" + to_string(minutes);
    }
    else {
        minute_string = to_string(minutes);
    }

    // We set the precision to 3, because otherwise there would be 6 digits after 0, which is undesirable.
    std::ostringstream streamObj3;
    streamObj3 << std::fixed;
    streamObj3 << std::setprecision(3);
    streamObj3 << seconds;
    std::string strObj3 = streamObj3.str();

    if (seconds < 10) {
        strObj3 = "0" + strObj3;
    }

    return minute_string + ":" + strObj3;
}

std::string Tema2::placeToFormat() {
    int place = findPlace();
    if (place == 1) return "1st";
    else if (place == 2) return "2nd";
    else if (place == 3) return "3rd";
    else return to_string(place) + "th";
}

void Tema2::RenderHUD() {
    if (raceIsOn) {
        // Rendering the lap counter.
        textRenderer->RenderText("Lap " + to_string((lap <= 0) ? 1 : lap) + " / " + to_string(totalLaps), 21 * window->GetResolution().x / 25, window->GetResolution().y / 30, 0.22, BLACK);

        // Rendering the timer
        textRenderer->RenderText("Total time: " + timeToFormat(totalTime), 18 * window->GetResolution().x / 25, 3 * window->GetResolution().y / 30, 0.2, BLACK);

        // Rendering the best lap
        if (bestLapTime == 0.0f) {
            textRenderer->RenderText("Best lap:   --:--.---", 18 * window->GetResolution().x / 25, 5 * window->GetResolution().y / 30, 0.2, BLACK);
        }
        else {
            textRenderer->RenderText("Best lap:   " + timeToFormat(bestLapTime), 18 * window->GetResolution().x / 25, 5 * window->GetResolution().y / 30, 0.2, BLACK);
        }

        if (timeUntilStart > 0) {
            float time = timeUntilStart - floor(timeUntilStart);
            textRenderer->RenderText(to_string((int)ceil(timeUntilStart)), 12 * window->GetResolution().x / 25, 12 * window->GetResolution().y / 30, 1.4 * (timeUntilStart - floor(timeUntilStart) + 0.7));
        }

        if (timeUntilStart <= 0 && goTime > 0) {
            textRenderer->RenderText("GO!", 11 * window->GetResolution().x / 25, 12 * window->GetResolution().y / 30, 0.8);
        }


        if (gameMode == RACE) {
            textRenderer->RenderText(placeToFormat(), window->GetResolution().x / 25, 2 * window->GetResolution().y / 30, 0.3, BLACK);
        }

        textRenderer->RenderText(to_string((int)(playerSpeed * 30)) + "Km/h", window->GetResolution().x / 25, 27 * window->GetResolution().y / 30, 0.23);
    }
    else if (showMenu == 1) {
        textRenderer->RenderText("START GAME", 0.32 * window->GetResolution().x, 14 * window->GetResolution().y / 30, 0.6);
        textRenderer->RenderText("EXIT", 0.42 * window->GetResolution().x, 18 * window->GetResolution().y / 30, 0.6);
    }
    else if (showMenu == 2) {
        textRenderer->RenderText("CLASSIC", 0.37 * window->GetResolution().x, 14 * window->GetResolution().y / 30, 0.6);
        textRenderer->RenderText("RACE", 0.42 * window->GetResolution().x, 18 * window->GetResolution().y / 30, 0.6);
        textRenderer->RenderText("BACK", 0.42 * window->GetResolution().x, 22 * window->GetResolution().y / 30, 0.6);
    }
    else if (showMenu == 3) {
        if (gameMode == RACE) {
            textRenderer->RenderText(placeToFormat() + " place", 0.32 * window->GetResolution().x, 2 * window->GetResolution().y / 30, 0.7, BLACK);
        }

        textRenderer->RenderText("Total time: " + timeToFormat(totalTime), 5 * window->GetResolution().x / 25, 7 * window->GetResolution().y / 30, 0.5);
        textRenderer->RenderText("Best lap:   " + timeToFormat(bestLapTime), 5 * window->GetResolution().x / 25, 16 * window->GetResolution().y / 30, 0.5);
        textRenderer->RenderText("TRY AGAIN", 0.415 * window->GetResolution().x, 24 * window->GetResolution().y / 30, 0.3);
        textRenderer->RenderText("MENU", 23 * window->GetResolution().x / 50, 26 * window->GetResolution().y / 30, 0.3);
        textRenderer->RenderText("EXIT", 23 * window->GetResolution().x / 50, 28 * window->GetResolution().y / 30, 0.3);
    }
}

void Tema2::Update(float deltaTimeSeconds)
{
    if (deltaTimeSeconds < 1) {
        if (raceIsOn) {
            if (timeUntilStart > 0) {
                timeUntilStart -= deltaTimeSeconds;
            }
            else {
                totalTime += deltaTimeSeconds;
                lapTime += deltaTimeSeconds;

                if (penaltyTime > 0) {
                    penaltyTime -= deltaTimeSeconds;
                }

                if (goTime > 0) {
                    goTime -= deltaTimeSeconds;
                }
            }
        }
    }

    // If the race is finished, then everything stops
    if (raceIsOn) {
        if (timeUntilStart <= 0) {
            moveCars(deltaTimeSeconds);

            // We add inertia to moving the car.
            if (playerSpeed > 0) {
                if (playerSpeed >= 4) {
                    playerSpeed -= playerSpeed * playerInertia * deltaTimeSeconds / 3;
                }
                else {
                    playerSpeed -= playerSpeed * playerInertia * deltaTimeSeconds * 2 / 3;
                }
                if (playerSpeed < 0.02) playerSpeed = 0;
            }
            else if (playerSpeed < 0) {
                playerSpeed += (-playerSpeed) * playerInertia * deltaTimeSeconds * 2 / 3;
                playerSpeed = min(playerSpeed, 0.0f);

                if (playerSpeed > -0.02) playerSpeed = 0;
            }
            movePlayerCar(playerSpeed * deltaTimeSeconds);
        }
    }

    // We position and render the player's view.
    positionCamera();
    RenderScene();

    RenderHUD();

    // We position and render the minimap.
    RenderMiniMap();
    positionMinimapCamera();
    RenderScene();
}


void Tema2::FrameEnd()
{
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();

    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

// We overload the RenderMesh method in order to also give the color to different models.
void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3& color) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader, the specified position and the specified color.
    shader->Use();
    int location = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(location, 1, glm::value_ptr(color));

    location = glGetUniformLocation(shader->program, "car_position");
    glUniform3fv(location, 1, glm::value_ptr(playerCar.position));
    glm::vec4 aux = playerModelMatrix * glm::vec4(playerCar.position, 1.0);
    glm::vec3 newCar = glm::vec3(aux.x, aux.y, aux.z);

    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

bool Tema2::verifyStart() {
    // If the player tries to jump start, then we give him a 5 second penalty.
    if (timeUntilStart > 0) {
        penaltyTime = 5;
        return false;
    }

    return true;
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float mouseSenzitivity = 3;
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            camera->MoveForward(mouseSenzitivity * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->TranslateRight(-mouseSenzitivity * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            camera->MoveForward(-mouseSenzitivity * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera->TranslateRight(mouseSenzitivity * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            camera->TranslateUpward(-mouseSenzitivity * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            camera->TranslateUpward(mouseSenzitivity * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_W) && raceIsOn && penaltyTime <= 0) {
        if (playerSpeed < playerSpeedMax && verifyStart()) {
            playerSpeed += playerAcceleration * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_S) && raceIsOn && penaltyTime <= 0) {
        if (playerSpeed > -playerSpeedMax && verifyStart()) {
            playerSpeed -= playerAcceleration * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_D) && raceIsOn && penaltyTime <= 0) {
        if (verifyStart()) {
            playerCar.rotateCar(playerRotationSpeed * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_A) && raceIsOn && penaltyTime <= 0) {
        if (verifyStart()) {
            playerCar.rotateCar(-playerRotationSpeed * deltaTime);
        }
    }
}


void Tema2::OnKeyPress(int key, int mods)
{  
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateFirstPerson_OY(-sensivityOX * deltaX);
            camera->RotateFirstPerson_OX(-sensivityOY * deltaY);
        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (showMenu == 1) {
        if (mouseX >= 0.323 * window->GetResolution().x && mouseX <= 0.677 * window->GetResolution().x &&
            mouseY >= 0.464 * window->GetResolution().y && mouseY <= 0.544 * window->GetResolution().y) {
            showMenu = 2;
        }
        else if (mouseX >= 0.423 * window->GetResolution().x && mouseX <= 0.563 * window->GetResolution().x &&
            mouseY >= 0.597 * window->GetResolution().y && mouseY <= 0.678 * window->GetResolution().y) {
            window->Close();
        }
    }
    // We select the game mode.
    else if (showMenu == 2) {
        if (mouseX >= 0.373 * window->GetResolution().x && mouseX <= 0.618 * window->GetResolution().x &&
            mouseY >= 0.464 * window->GetResolution().y && mouseY <= 0.546 * window->GetResolution().y) {
            gameMode = CLASSIC;
            InitializeGame();
        }
        else if (mouseX >= 0.423 * window->GetResolution().x && mouseX <= 0.563 * window->GetResolution().x &&
            mouseY >= 0.597 * window->GetResolution().y && mouseY <= 0.678 * window->GetResolution().y) {
            gameMode = RACE;
            InitializeGame();
        }
        else if (mouseX >= 0.423 * window->GetResolution().x && mouseX <= 0.563 * window->GetResolution().x &&
            mouseY >= 0.731 * window->GetResolution().y && mouseY <= 0.811 * window->GetResolution().y) {
            showMenu = 1;
        }
    }
    // If the race has finished, we can press the try again button to restart the race.
    else if (showMenu == 3) {
        if (mouseX >= 0.414 * window->GetResolution().x && mouseX <= 0.574 * window->GetResolution().x &&
            mouseY >= 0.799 * window->GetResolution().y && mouseY <= 0.838 * window->GetResolution().y) {
            InitializeGame();
        }
        else if (mouseX >= 0.46 * window->GetResolution().x && mouseX <= 0.53 * window->GetResolution().x &&
            mouseY >= 0.865 * window->GetResolution().y && mouseY <= 0.907 * window->GetResolution().y) {
            showMenu = 1;
        }
        else if (mouseX >= 0.46 * window->GetResolution().x && mouseX <= 0.53 * window->GetResolution().x &&
            mouseY >= 0.931 * window->GetResolution().y && mouseY <= 0.972 * window->GetResolution().y) {
            window->Close();
        }
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}

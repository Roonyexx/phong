#pragma once 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.hpp"

class Camera
{
protected:
    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 orientation;

    glm::mat4 view;
    glm::mat4 proj;

    double sensitivity;

public:
    Camera(glm::vec3 position, glm::mat4 proj);

    void setCameraMatrix(Shader& shader, char const* uniform);

    void cameraMove(glm::mat4 m);

    void rotateArounOrigin(float angle, glm::vec3 const& trans);
};

class OrbitalCamera : public Camera
{
private:
    float yaw;
    float pitch;
    float radius;
    double lastMouseX, lastMouseY;
    void updatePositionFromOrbit();
    

public:
    OrbitalCamera(glm::vec3 position, glm::mat4 proj);
    void orbitDelta(float deltaYaw, float deltaPitch);
    void inputs(GLFWwindow* window);
};

#include "Camera.hpp"
#include <iostream>
Camera::Camera(glm::vec3 position, glm::mat4 proj) : position{ position }, proj{ proj }, up{ 0.0f, 1.0f, 0.0f }, orientation{ 0.0f, 0.0f, -1.0f }, sensitivity{20.0f}, speed{ 2.0f }
{ }

void Camera::setCameraMatrix(Shader& shader, char const* uniform)
{
    view = glm::mat4{ 1.0f };
    view = glm::lookAt(position, orientation + position, up);
    glUniformMatrix4fv(glGetUniformLocation(shader.id, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(glGetUniformLocation(shader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(glGetUniformLocation(shader.id, "camPos"), position.x, position.y, position.z);
}

void Camera::cameraMove(glm::mat4 m)
{
    glm::vec4 transformed = m * glm::vec4(position, 1.0f);
    position = glm::vec3(transformed);

    orientation = glm::vec3(-position);
    up = glm::vec3(m * glm::vec4(up, 0.0f));
}

void Camera::rotateArounOrigin(float angle, glm::vec3 const &trans)
{
    glm::mat4 rot{ glm::rotate(glm::mat4(1.0f), angle, trans) };

    glm::vec4 newPos = rot * glm::vec4(position, 1.0f);
    position = glm::vec3(newPos) / newPos.w;

    orientation = glm::vec3(-position);

    up = glm::vec3(rot * glm::vec4(up, 0.0f));
}

void OrbitalCamera::updatePositionFromOrbit()
{
    float x = radius * cos(pitch) * sin(yaw);
    float y = radius * sin(pitch);
    float z = radius * cos(pitch) * cos(yaw);
    position = glm::vec3(x, y, z);

    orientation = glm::normalize(-position);
}

OrbitalCamera::OrbitalCamera(glm::vec3 position, glm::mat4 proj) : Camera(position, proj), yaw{ 0 }, pitch{ 0 }, radius{ position.z }, lastMouseY{}, lastMouseX{}
{ }

void OrbitalCamera::orbitDelta(float deltaYaw, float deltaPitch)
{
    yaw += deltaYaw;
    pitch += deltaPitch;

    const float limit = glm::radians(89.0f);
    if (pitch >  limit) pitch =  limit;
    if (pitch < -limit) pitch = -limit;

    updatePositionFromOrbit();
}

void OrbitalCamera::inputs(GLFWwindow *window, double dt)
{
    float speedMult{ 1.0f };
    int state{ glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) };
    if (state == GLFW_PRESS)
    {
        speedMult = 3.0f;
    }
    
    state = glfwGetKey(window, GLFW_KEY_W);
    if (state == GLFW_PRESS)
    {
        radius -= speed * speedMult * dt;
        updatePositionFromOrbit();
    }
    state = glfwGetKey(window, GLFW_KEY_S);
    if (state == GLFW_PRESS)
    {
        radius += speed * speedMult * dt;
        updatePositionFromOrbit();
    }

    static bool firstClick{ true };
    state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if(state == GLFW_PRESS)
    {
        double mouseX{}, mouseY{};
        glfwGetCursorPos(window, &mouseX, &mouseY);
        if (firstClick) { lastMouseX = mouseX; lastMouseY = mouseY; firstClick = false; }
        orbitDelta(glm::radians((lastMouseX - mouseX) * sensitivity * dt), 
                   -glm::radians((lastMouseY - mouseY) * sensitivity * dt));
        lastMouseX = mouseX; lastMouseY = mouseY;
    }
    else if(state == GLFW_RELEASE)
    {
        firstClick = true;
    }
}

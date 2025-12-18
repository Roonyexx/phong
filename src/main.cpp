#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "Curve.hpp"
#include "ObjectByCurveRotate.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

int currentScene = 1;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0,0,width,height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_1) currentScene = 1;
        else if (key == GLFW_KEY_2) currentScene = 2;
        else if (key == GLFW_KEY_3) currentScene = 3;
        else if (key == GLFW_KEY_4) currentScene = 4;
        else if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, true);
    }
}

int main() 
{
    int const width{ 800 }, height{ 800 }; 
    if (!glfwInit()) { std::cerr << "glfwInit failed\n"; return -1; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width, height, "pusa", nullptr, nullptr);
    if (!window) { std::cerr<<"failed to create window\n"; glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { std::cerr<<"failed to init glad\n"; return -1; }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    Curve halfCircle{ getHalfCircle(1.0f, {0.0f,0.0f,0.0f}, 18) };
    Curve triangle{ {
        Vertex{ 0.0f, -1.0f, 0.0f },
        Vertex{ 1.0f,  -1.0f, 0.0f },
        Vertex{ 0.0f,  1.0f, 0.0f }
    } };
    Curve rectangle{ {
        Vertex{ 0.0f, -1.0f, 0.0f },
        Vertex{ -1.0f,  -1.0f, 0.0f },
        Vertex{ -1.0f,  1.0f, 0.0f },
        Vertex{ 0.0f, 1.0f, 0.0f }
    } };

    ObjectByCurveRotate ball{ halfCircle, 10.0f };
    ObjectByCurveRotate cone{ triangle, 5.0f };
    ObjectByCurveRotate cylinder{ rectangle, 5.0f };

    Shader shaderProgram("res/shaders/default.vert", "res/shaders/default.frag");
    shaderProgram.linkGeometryShader("res/shaders/default.geom");

    VAO vaoBall;
    vaoBall.bind();
    auto vertsBall = ball.getVerts();
    auto indicesBall = ball.getIndices();
    VBO vboBall(vertsBall.data(), static_cast<GLsizeiptr>(vertsBall.size() * sizeof(Vertex)));
    EBO eboBall(indicesBall.data(), static_cast<GLsizeiptr>(indicesBall.size() * sizeof(uint32_t)));
    vaoBall.linkAttrib(vboBall, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
    vaoBall.linkAttrib(vboBall, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    vaoBall.unbind();

    VAO vaoCone;
    vaoCone.bind();
    auto vertsCone = cone.getVerts();
    auto indicesCone = cone.getIndices();
    VBO vboCone(vertsCone.data(), static_cast<GLsizeiptr>(vertsCone.size() * sizeof(Vertex)));
    EBO eboCone(indicesCone.data(), static_cast<GLsizeiptr>(indicesCone.size() * sizeof(uint32_t)));
    vaoCone.linkAttrib(vboCone, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
    vaoCone.linkAttrib(vboCone, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    vaoCone.unbind();

    VAO vaoCylinder;
    vaoCylinder.bind();
    auto vertsCylinder = cylinder.getVerts();
    auto indicesCylinder = cylinder.getIndices();
    VBO vboCylinder(vertsCylinder.data(), static_cast<GLsizeiptr>(vertsCylinder.size() * sizeof(Vertex)));
    EBO eboCylinder(indicesCylinder.data(), static_cast<GLsizeiptr>(indicesCylinder.size() * sizeof(uint32_t)));
    vaoCylinder.linkAttrib(vboCylinder, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
    vaoCylinder.linkAttrib(vboCylinder, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    vaoCylinder.unbind();

    glm::mat4 proj{ glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f) };
    GLint modelLoc{ glGetUniformLocation(shaderProgram.id, "model") };

    OrbitalCamera camera{ {0.0f, 0.0f, 6.0f}, proj };

    glEnable(GL_DEPTH_TEST);
    double lastTime{ glfwGetTime() };
    
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.9f,0.9f,0.9f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        double curTime{ glfwGetTime() };
        double dt { curTime - lastTime };
        lastTime = curTime;

        camera.inputs(window, dt);
        shaderProgram.activate();
        camera.setCameraMatrix(shaderProgram, "camMat");

        glm::mat4 model{ 1.0f };
        switch (currentScene)
        {
        case 1:
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            vaoBall.bind();
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indicesBall.size()), GL_UNSIGNED_INT, 0);
            break;
        case 2:
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            vaoCone.bind();
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indicesCone.size()), GL_UNSIGNED_INT, 0);
            break;
        case 3:
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            vaoCylinder.bind();
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indicesCylinder.size()), GL_UNSIGNED_INT, 0);
            break;
        case 4:
            glm::mat4 modelBall{ glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) };
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBall));
            vaoBall.bind();
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indicesBall.size()), GL_UNSIGNED_INT, 0);

            glm::mat4 modelCone{ glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f)) };
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCone));
            vaoCone.bind();
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indicesCone.size()), GL_UNSIGNED_INT, 0);

            glm::mat4 modelCylinder{ glm::translate(glm::mat4(1.0f), glm::vec3(2.3f, 0.0f, 0.0f)) };
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCylinder));
            vaoCylinder.bind();
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indicesCylinder.size()), GL_UNSIGNED_INT, 0);
            break;
        default:
            break;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

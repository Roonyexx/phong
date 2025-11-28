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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0,0,width,height);
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

    ObjectByCurveRotate& obj{ ball };


    Shader shaderProgram("res/shaders/default.vert", "res/shaders/default.frag");

    VAO vao;
    vao.bind();

    auto verts = obj.getVerts();
    auto indices = obj.getIndices();

    VBO vbo(verts.data(), static_cast<GLsizeiptr>(verts.size() * sizeof(Vertex)));
    EBO ebo(indices.data(), static_cast<GLsizeiptr>(indices.size() * sizeof(uint32_t)));

    vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
    vao.linkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)offsetof(Vertex, normal));

    glm::mat4 model{ 1.0f }, proj{ glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f) };
    GLint modelLoc{ glGetUniformLocation(shaderProgram.id, "model") };

    OrbitalCamera camera{ {0.0f, 0.0f, 6.0f}, proj };


    vao.unbind();
    vbo.unbind();
    ebo.unbind();

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
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        camera.setCameraMatrix(shaderProgram, "camMat");

        vao.bind();
        GLsizei indexCount = static_cast<GLsizei>(indices.size());
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        GLuint err = glGetError();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
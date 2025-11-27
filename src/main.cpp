#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
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
    GLFWwindow* window = glfwCreateWindow(width, height, "pusa", nullptr, nullptr);
    if (!window) { std::cerr<<"failed to create window\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { std::cerr<<"failed to init glad\n"; return -1; }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    
    glm::mat4 identityMatrix(1.0f);

    Curve halfCircle{ getHalfCircle(1.0f, {0.0f,0.0f,0.0f}, 5) };
    for (auto const& vert : halfCircle.verts)
    {
        std::cout << vert.cord.x << " " << vert.cord.y << " " << vert.cord.z << std::endl;
    }

    ObjectByCurveRotate obj{ halfCircle, 90.0f };

    GLfloat vertices[] = 
    {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, 0.0f, 128 / 255.0f, 255 / 255.0f, 
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, 255 / 255.0f, 0 / 255.0f, 0 / 255.0f,
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, 255.0f, 153 / 255.0f, 0 / 255.0f
    };

    GLuint indices[] = 
    {
        0, 1, 2
    };

    Shader shaderProgram("res/shaders/default.vert", "res/shaders/default.frag");

    VAO vao;
    vao.bind();

    VBO vbo(vertices, sizeof(vertices));
    EBO ebo(indices, sizeof(indices));

    vao.linkAttrib(vbo, 0, 3, GL_FLOAT, 6*sizeof(float), (void *)0);
    vao.linkAttrib(vbo, 1, 3, GL_FLOAT, 6*sizeof(float), (void *)(3 * sizeof(float)));

    glm::mat4 model{ 1.0f }, view{ 1.0f }, proj{ glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f) };
    GLint modelLoc{ glGetUniformLocation(shaderProgram.id, "model") }, 
          viewLoc{ glGetUniformLocation(shaderProgram.id, "view") }, 
          projLoc{ glGetUniformLocation(shaderProgram.id, "proj") };

    view = glm::translate(view, { 0.0f, 0.0f, -3.0f });


    vao.unbind();
    vbo.unbind();
    ebo.unbind();

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.9f,0.9f,0.9f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model = glm::rotate(model, glm::radians(1.0f), { 0.0f, 1.0f, 0.0f });

        shaderProgram.activate();
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        vao.bind();
        glDrawElements(GL_TRIANGLE_FAN, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
        GLuint err = glGetError();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }   


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}



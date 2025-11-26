#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>
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
    if (!glfwInit()) { std::cerr << "glfwInit failed\n"; return -1; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(800, 800, "я треугольник 3 часа рисовал...", nullptr, nullptr);
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
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f
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

    vao.linkAttrib(vbo, 0, 3, GL_FLOAT, 3*sizeof(float), (void *)0);

    vao.unbind();
    vbo.unbind();
    ebo.unbind();

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.9f,0.9f,0.9f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.activate();
        vao.bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        GLuint err = glGetError();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }   


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}



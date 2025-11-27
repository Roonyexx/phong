#pragma once
#include <glad/glad.h>

class VBO
{
public:
    GLuint id;
    VBO(void const* vertices, GLsizeiptr size);
    void bind();
    void unbind();
    void remove();
};
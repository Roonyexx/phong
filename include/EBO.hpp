#pragma once 
#include <glad/glad.h>

class EBO
{
public:
    GLuint id;

    EBO(void const *indices, GLsizeiptr size);
    void bind();
    void unbind();
    void remove();
};
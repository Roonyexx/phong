#pragma once
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 cord{};
    glm::vec3 normal{};
    Vertex(float x, float y, float z) : cord{x, y, z} { } 
};

inline Vertex transformVertex(Vertex in, glm::mat4 const& M)
{
    Vertex out { in };
    glm::vec4 p{ M * glm::vec4{ in.cord, 1.0f } };
    out.cord = glm::vec3{ p };
    return out;
}
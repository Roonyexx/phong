#pragma once
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 cord{};
    glm::vec3 normal{};
};

inline Vertex transformVertex(Vertex const& in, glm::mat4 const& M)
{
    Vertex out { in };
    glm::vec4 p{ M * glm::vec4{ in.cord, 1.0f } };
    out.cord = glm::vec3{ p };
    return out;
}
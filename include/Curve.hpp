#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Vertex.hpp"
#include <numbers>


using Edge = std::pair<int, int>;

struct Curve
{
    std::vector<Vertex> verts;

    Curve(const std::vector<Vertex>& v) : verts{ v } { }
    Curve() = default;
};

inline glm::vec3 circlePoint(float radius, glm::vec3 const& center, glm::vec2 const& yz) //(x-x)^2 + (y-y)^2 = r^2
{
    float dy{ yz[0] - center.y };
    float dz{ yz[1] - center.z };

    float inside{ radius * radius - dy * dy - dz * dz };
    inside = std::max(inside, 0.0f);
    float x{ center.x + sqrt(inside) };

    return { x, yz[0], yz[1] };
}

inline Curve getHalfCircle(float radius=1.0f, glm::vec3 const& center={0.0f,0.0f,0.0f}, uint8_t numOfPartions=6)
{
    Curve curve; float angle{ 180.0f / numOfPartions }; 
    glm::mat4 M{ glm::translate(glm::mat4(1.0f), center)};
    M = glm::rotate(M, glm::radians(angle), {0.0f, 0.0f, 1.0f});
    curve.verts.push_back({center.x, center.y - radius, center.z});
    for(size_t i{}; i < numOfPartions; i++)
    {
        Vertex vert = transformVertex(curve.verts[i], M);
        curve.verts.push_back(vert);
    }
    return curve;
}
#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Vertex.hpp"

using Edge = std::pair<int, int>;

struct Curve
{
    std::vector<Vertex> verts;
    std::vector<Edge> edges;
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
    float step{ 2 * radius / numOfPartions };
    Curve curve; float y{ center.y - radius };
    for(size_t i{}; i <= numOfPartions; i++)
    {
        Vertex vert;
        vert.cord = circlePoint(radius, center, {y, center.z});
        curve.verts.push_back(vert);
        y += step;
    }

    // for(size_t i{}, len{ numOfPartions }; i < len; i++)
    //     curve.edges.push_back({i, i+1});
    return curve;
}
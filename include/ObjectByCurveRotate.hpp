#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Curve.hpp"

class ObjectByCurveRotate
{
private:
    Curve curve;
    std::vector<Vertex> verts;
    std::vector<Edge> edges;
    std::vector<uint32_t> indices;

    void curveRotate(float const angle);
    void triangle(size_t start, size_t end, uint16_t per);
public:
    ObjectByCurveRotate(Curve const& curve, float const rotAngle);
};
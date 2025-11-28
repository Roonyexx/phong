#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Curve.hpp"

class ObjectByCurveRotate
{
private:
    Curve curve;
    std::vector<Vertex> verts;
    //std::vector<Edge> edges;
    std::vector<uint32_t> indices;

    void curveRotate(float const angle);
    void triangle(size_t firstCurveStart, size_t secondCurveStart, uint32_t per);
    void normalsCalculate();
public:
    ObjectByCurveRotate(Curve const& curve, float const rotAngle);
    
    std::vector<Vertex> getVerts() const;
    std::vector<uint32_t> getIndices() const;
};
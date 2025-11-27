#include "ObjectByCurveRotate.hpp"

void ObjectByCurveRotate::curveRotate(float const angle)
{
    if (angle <= 0.0f) return;
    const size_t profileSize{ curve.verts.size() };
    
    int steps = static_cast<int>(360.0f / angle);
    if (steps <= 2) return;

    verts.clear();
    indices.clear();

    verts.insert(verts.end(), curve.verts.begin(), curve.verts.end());

    for (size_t s{ 1 }; s < steps; s++)
    {
        glm::mat4 rotateMat { glm::rotate(glm::mat4{ 1.0f }, 
                              glm::radians(angle * s),
                              glm::vec3{ 0.0f, 1.0f, 0.0f}) };

        for(auto const& vert : curve.verts)
        {
            Vertex v = vert;
            glm::vec4 p = rotateMat * glm::vec4{ vert.cord, 1.0f };
            v.cord = glm::vec3(p) / p.w;
            this->verts.push_back(v);
        }

        this->triangle(profileSize * (s - 1), profileSize * s, profileSize - 1); // c.verts.size = 6 steps = 4; 1(0, 6) 2(6, 12) 3(12, 18) 
    }
    this->triangle(profileSize * (steps - 1), 0, profileSize - 1);
}

void ObjectByCurveRotate::triangle(size_t firstCurveStart, size_t secondCurveStart, uint32_t per)
{
    struct Triangle 
    {
        uint32_t A, B, C;
    };

    for(size_t i{}; i < per; i++) //0 1 7, 0 6 7; 1 2 8, 1 7 8; 2 3 9, 2 8 9;
    {
        Triangle t1{ firstCurveStart + i, secondCurveStart + i, firstCurveStart + i + 1 };
        Triangle t2{ firstCurveStart + i + 1, secondCurveStart + i, secondCurveStart + i + 1 };

        indices.push_back(t1.A); indices.push_back(t1.B); indices.push_back(t1.C);
        indices.push_back(t2.A); indices.push_back(t2.B); indices.push_back(t2.C);
    }
}

ObjectByCurveRotate::ObjectByCurveRotate(Curve const& curve, float const rotAngle)  : curve{ curve }
{
    curveRotate(rotAngle);
}

std::vector<Vertex> ObjectByCurveRotate::getVerts() const
{
    return verts;
}

std::vector<uint32_t> ObjectByCurveRotate::getIndices() const
{
    return indices;
}

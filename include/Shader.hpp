#pragma once 
#include <glad/glad.h>
#include <string>
#include <fstream>

inline std::string getFileContent(std::string const& filename);
class Shader
{
public:
    GLuint id;

    Shader(std::string const& vertexFile, std::string const& fragmentfile);
    void linkGeometryShader(std::string const& geometryFile);
    void activate();
    void remove();
};
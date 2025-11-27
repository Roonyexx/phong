#include "Shader.hpp"

inline std::string getFileContent(std::string const &filename)
{
    std::ifstream in{ filename, std::ios::binary };
    if(in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return (contents);
    }
    throw(errno);
}

Shader::Shader(std::string const &vertexFile, std::string const &fragmentFile)
{
    std::string vertexCode{ getFileContent(vertexFile) };
    std::string fragmentCode{ getFileContent(fragmentFile) };

    const char *vertexSource{ vertexCode.c_str() };
    const char *fragmentSource{ fragmentCode.c_str() };

    GLuint vertexShader{ glCreateShader(GL_VERTEX_SHADER) };
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);


    GLuint fragmentShader{ glCreateShader(GL_FRAGMENT_SHADER) };
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);    

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::activate()
{
    glUseProgram(id);
}

void Shader::remove()
{
    glDeleteProgram(id);
}

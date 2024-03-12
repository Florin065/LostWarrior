#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Core/Graphics/Shader.hpp"

#include "Core/Coordinator.hpp"


extern Coordinator gCoordinator;


void readFileIntoString(std::string const& path, std::string &str)
{
    std::ifstream file;
    file.open(path);
    std::stringstream fileStream;
    fileStream << file.rdbuf();
    file.close();
    str = fileStream.str();

    gCoordinator.Assert(!file.fail(), "Could not read file: ", path);
}

GLuint compileShader(const GLchar* source, GLenum type)
{
    GLint success;
    GLchar infoLog[1024];

    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        gCoordinator.LogError("Could not compile shader: ", infoLog);
        exit(EXIT_FAILURE);
    }

    const std::string typeName = type == GL_VERTEX_SHADER   ? "Vertex"
                               : type == GL_FRAGMENT_SHADER ? "Fragment"
                               : type == GL_GEOMETRY_SHADER ? "Geometry"
                               : "Undefined";

    gCoordinator.LogInfo(typeName, " shader compiled succesfully!");

    return shader;
}

Shader::Shader(std::string const& vertPath,
               std::string const& fragPath,
               std::string const& geomPath)
{
    GLint success;
    GLchar infoLog[1024];

    std::string vertSource, fragSource, geomSource;
    GLuint vertShader, fragShader, geomShader = 0;

    readFileIntoString(vertPath, vertSource);
    vertShader = compileShader(vertSource.c_str(), GL_VERTEX_SHADER);

    readFileIntoString(fragPath, fragSource);
    fragShader = compileShader(fragSource.c_str(), GL_FRAGMENT_SHADER);

    mId = glCreateProgram();
    glAttachShader(mId, vertShader);
    glAttachShader(mId, fragShader);

    if (!geomPath.empty())
    {
        readFileIntoString(geomPath, geomSource);
        geomShader = compileShader(geomSource.c_str(), GL_GEOMETRY_SHADER);
        glAttachShader(mId, geomShader);
    }

    glLinkProgram(mId);

    glGetProgramiv(mId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(mId, sizeof(infoLog), nullptr, infoLog);
        gCoordinator.LogError("Could not link shaders: ", infoLog);
        exit(EXIT_FAILURE);
    }
    gCoordinator.LogInfo("Shader program linked succesfully!");

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    if (!geomPath.empty())
    {
        glDeleteShader(geomShader);
    }
}

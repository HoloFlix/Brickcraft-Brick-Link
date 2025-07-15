#include "ShaderLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

GLuint ShaderLoader::Load(const std::string& vertexPath, const std::string& fragmentPath) {
    std::ifstream vFile(vertexPath), fFile(fragmentPath);
    std::stringstream vStream, fStream;
    vStream << vFile.rdbuf();
    fStream << fFile.rdbuf();
    std::string vCode = vStream.str(), fCode = fStream.str();

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    const char* vStr = vCode.c_str();
    glShaderSource(vertex, 1, &vStr, nullptr);
    glCompileShader(vertex);

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fStr = fCode.c_str();
    glShaderSource(fragment, 1, &fStr, nullptr);
    glCompileShader(fragment);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}
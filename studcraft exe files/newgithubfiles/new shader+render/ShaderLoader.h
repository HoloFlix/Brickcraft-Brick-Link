#pragma once
#include <string>
#include <GL/glew.h>

class ShaderLoader {
public:
    static GLuint Load(const std::string& vertexPath, const std::string& fragmentPath);
};
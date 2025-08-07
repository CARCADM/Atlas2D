#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"

#include "string"
#include "fstream"
#include "sstream"
#include "iostream"

struct shader
{
    unsigned int ID;

    shader() = default;
    shader(const char* vertexPath, const char* fragmentPath);

    void use();
    void bindShaderProgram(unsigned int& shaderProgram);

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};

#endif
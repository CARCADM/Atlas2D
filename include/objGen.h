#ifndef OBJGEN_H
#define OBJGEN_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>
#include <vector>

struct objGen
{
    static int generateVBO(unsigned int& VBO, int attribute, const std::vector<float> vertices, GLenum usage);

    static int generateVBO(unsigned int& VBO, int attribute, const std::vector<float> vertices, GLenum usage, unsigned int texture);
    
    static void generateVAO(unsigned int& VAO);

    static void generateEBO(unsigned int& EBO, std::vector<unsigned int> indicies, GLenum usage);

    static int generateTexture(unsigned int& texture2D, const char* texturePath, int& width, int& height);
};

#endif

// TODO implement generateEBO